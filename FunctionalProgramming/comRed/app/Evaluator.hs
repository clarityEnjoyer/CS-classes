module Evaluator 
  ( generateHistory
  ) where

import Types
import qualified Data.Map as Map

-- ==============================================================================
-- Zipper logic
-- ==============================================================================
-- Ctx holds a path to the current place.
data Ctx = Top 
         | AppLeft Ctx Expr 
         | AppRight Expr Ctx 
         deriving (Show, Eq)

-- Loc is current subtree + context ("supertree").
data Loc = Loc { locExpr :: Expr, locCtx :: Ctx } deriving (Show, Eq)

-- Expression fixing by retreating zipper to the root.
plug :: Loc -> Expr
plug (Loc expr Top) = expr
plug (Loc expr (AppLeft ctx rightArg)) = plug (Loc (expr :$ rightArg) ctx)
plug (Loc expr (AppRight leftFun ctx)) = plug (Loc (leftFun :$ expr) ctx)

-- Zipper travels to precisely picked place.
-- For example: for `add Z (S Z)` i targetIdx=1, skupi się na `(S Z)`.
-- NB: 'list !! i' is used to access i-th element of the list. 
-- Although, it has linear time complexity, we usually have little arguments so our lists will be short. 
makeArgLoc :: Expr -> [Expr] -> Int -> Ctx -> Loc
makeArgLoc headExpr args targetIdx outerCtx = 
    let finalCtx = buildCtx (length args - 1) outerCtx
    in Loc (args !! targetIdx) finalCtx
  where
    buildCtx currentIdx ctx
      | currentIdx == targetIdx = 
          let leftSide = foldl (:$) headExpr (take currentIdx args)
          in AppRight leftSide ctx
      | otherwise = 
          let rightArg = args !! currentIdx
          in buildCtx (currentIdx - 1) (AppLeft ctx rightArg)


-- ==============================================================================
-- State monad holding DefMap, fuel and history accumulator
-- ==============================================================================
newtype EvalM a = EvalM { runEvalM :: DefMap -> Int -> SnocList Expr -> (a, Int, SnocList Expr) }

instance Functor EvalM where
  fmap f (EvalM m) = EvalM (\defs fuel hist -> 
    let (wynik, nowePaliwo, nowaHist) = m defs fuel hist 
    in (f wynik, nowePaliwo, nowaHist))

instance Applicative EvalM where
  pure x = EvalM (\_ fuel hist -> (x, fuel, hist))
  EvalM mf <*> EvalM mx = EvalM (\defs fuel hist ->
    let (funkcja, fuel1, hist1) = mf defs fuel hist
        (wynik, fuel2, hist2) = mx defs fuel1 hist1
    in (funkcja wynik, fuel2, hist2))

instance Monad EvalM where
  EvalM m >>= f = EvalM (\defs fuel hist ->
    let (wynik, fuel1, hist1) = m defs fuel hist
        EvalM g = f wynik
    in g defs fuel1 hist1)

getFuel :: EvalM Int
getFuel = EvalM (\_ fuel hist -> (fuel, fuel, hist))

reduceFuel :: EvalM ()
reduceFuel = EvalM (\_ fuel hist -> ((), fuel - 1, hist))

logStep :: Expr -> EvalM ()
logStep expr = EvalM (\_ fuel hist -> ((), fuel, snoc hist expr))

getDefs :: EvalM DefMap
getDefs = EvalM (\defs fuel hist -> (defs, fuel, hist))


-- ==============================================================================
-- Helper functions
-- ==============================================================================
-- Flattens given application tree: (((f a) b) c) -> (f, [a,b,c])
flattenApp :: Expr -> (Expr, [Expr])
flattenApp expr = go expr []
  where
    go (l :$ r) acc = go l (r : acc)
    go base acc     = (base, acc)

-- Performs substitution
subst :: [(Name, Expr)] -> Expr -> Expr
subst env (Var name) = 
  case lookup name env of
    Just newExpr -> newExpr
    Nothing      -> Var name
subst env (l :$ r)   = subst env l :$ subst env r
subst _ (Con c)      = Con c
subst env (Focus e)  = Focus (subst env e)


-- ==============================================================================
-- Pattern matching engine
-- ==============================================================================
-- Represents the tri-state outcome of a lazy pattern match.
data PatMatchRes = PatMatch [(Name, Expr)] | PatFail | PatNeedEval
data TopMatchRes = TopMatch [(Name, Expr)] | TopFail | TopNeedEval Int

-- Matches a single pattern against an expression.
matchPat :: Pat -> Expr -> PatMatchRes
matchPat (PVar n) e = PatMatch [(n, e)]
matchPat (PApp pName pArgs) e =
  case flattenApp e of
    (Con cName, cArgs) -> 
      if pName == cName && length pArgs == length cArgs
      then matchPatList pArgs cArgs []
      else PatFail
    -- If the expression is not a constructor, it's an unreduced variable or application.
    -- We must force its evaluation to proceed with pattern matching.
    _ -> PatNeedEval 

-- Recursively matches a list of patterns against a list of arguments inside a constructor.
matchPatList :: [Pat] -> [Expr] -> [(Name, Expr)] -> PatMatchRes
matchPatList [] [] env = PatMatch env
matchPatList (p:ps) (e:es) env =
  case matchPat p e of
    PatMatch env' -> matchPatList ps es (env ++ env')
    PatFail       -> PatFail
    PatNeedEval   -> PatNeedEval
matchPatList _ _ _ = PatFail

-- Matches the top-level arguments of a function application.
-- Returns the exact index of the argument that blocked the matching process (TopNeedEval idx).
matchTopArgs :: [Pat] -> [Expr] -> [(Name, Expr)] -> Int -> TopMatchRes
matchTopArgs [] _ env _ = TopMatch env
matchTopArgs (p:ps) (e:es) env idx =
  case matchPat p e of
    PatMatch env' -> matchTopArgs ps es (env ++ env') (idx + 1)
    PatFail       -> TopFail
    PatNeedEval   -> TopNeedEval idx
matchTopArgs _ [] _ _ = TopFail


-- ==============================================================================
-- Core Evaluator Engine (Pure navigation, no monadic effects)
-- ==============================================================================
-- Searches for the leftmost-outermost redex and performs a single reduction step.
-- Returns a tuple: (Expression with Focus applied, Clean expression for next step).
stepLoc :: DefMap -> Loc -> Maybe (Expr, Expr)
stepLoc defs loc =
  case flattenApp (locExpr loc) of
    (Var fName, args) ->
      case Map.lookup fName defs of
        -- Function definition found, attempt to match its clauses.
        Just (Def matches) -> tryMatches defs fName matches args loc
        -- Unrecognized variable, attempt to deeply evaluate its arguments instead.
        Nothing            -> stepArgs defs (Var fName) args loc
    (Con cName, args) -> 
      -- Constructors are in WHNF, but we traverse their arguments to reach full Normal Form.
      stepArgs defs (Con cName) args loc
    _ -> Nothing

-- Attempts to match the arguments against the sequential clauses of a function definition.
tryMatches :: DefMap -> Name -> [Match] -> [Expr] -> Loc -> Maybe (Expr, Expr)
tryMatches defs fName [] args loc = 
  -- Exhausted all clauses. The expression is stuck. Fallback to evaluating arguments.
  stepArgs defs (Var fName) args loc 

tryMatches defs fName (m:ms) args loc =
  let expectedArgs = length (matchPats m)
  in if length args < expectedArgs 
     then stepArgs defs (Var fName) args loc -- Partial application, evaluate arguments.
     else 
       let (matchArgs, remArgs) = splitAt expectedArgs args
       in case matchTopArgs (matchPats m) matchArgs [] 0 of
            
            TopMatch env -> 
              -- REDEX FOUND! We rebuild two versions of the tree.
              -- 1. Logged version: We inject 'Focus' around the redex.
              -- 2. Clean version: We substitute variables in the RHS for the next iteration.
              let redex      = foldl (:$) (Var fName) matchArgs
                  loggedBody = foldl (:$) (Focus redex) remArgs
                  cleanBody  = foldl (:$) (subst env (matchRhs m)) remArgs
              in Just (plug (Loc loggedBody (locCtx loc)), plug (Loc cleanBody (locCtx loc)))
              
            TopFail -> 
              -- This clause failed (e.g. expected Z, got S). Try the next clause (ms).
              tryMatches defs fName ms args loc
              
            TopNeedEval idx -> 
              -- Lazy evaluation forced: Argument at 'idx' must be reduced to a constructor.
              -- We move the Zipper down to this argument and recursively call stepLoc.
              let argLoc = makeArgLoc (Var fName) args idx (locCtx loc)
              in case stepLoc defs argLoc of
                   Just res -> Just res
                   Nothing  -> tryMatches defs fName ms args loc -- Argument reached WHNF but still didn't match.

-- Sequentially attempts to find a reducible redex inside a list of arguments (Left-to-Right).
stepArgs :: DefMap -> Expr -> [Expr] -> Loc -> Maybe (Expr, Expr)
stepArgs defs headExpr args loc = go 0
  where
    go idx
      | idx >= length args = Nothing -- No arguments could be reduced.
      | otherwise =
          let argLoc = makeArgLoc headExpr args idx (locCtx loc)
          in case stepLoc defs argLoc of
               Just res -> Just res
               Nothing  -> go (idx + 1) -- This argument is clean, try the next one.


-- ==============================================================================
-- Main execution driver
-- ==============================================================================
-- Bootstraps the monadic evaluation loop and returns the accumulated history.
generateHistory :: DefMap -> Expr -> Int -> [Expr] -> [Expr]
generateHistory defs startExpr initialFuel _ =
  let (_, _, historySnoc) = runEvalM (evalLoop startExpr) defs initialFuel mempty
  in toList historySnoc
  where
    -- The core state-machine loop running inside the EvalM monad.
    evalLoop :: Expr -> EvalM ()
    evalLoop expr = do
      fuel <- getFuel
      if fuel <= 0
        then return ()
        else do
          dm <- getDefs
          -- Execute a pure reduction step starting from the root of the tree (Top).
          case stepLoc dm (Loc expr Top) of
            Just (loggedRoot, cleanRoot) -> do
              logStep loggedRoot
              reduceFuel
              evalLoop cleanRoot -- Tail-recursive call with the new clean tree.
            Nothing -> do
              -- Evaluation reached Normal Form (no more possible reductions).
              logStep expr