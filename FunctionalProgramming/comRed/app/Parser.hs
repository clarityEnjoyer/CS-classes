-- f a b = 1
-- f a b = 2
-- or
-- f (S x) A = F 
-- f (S x) A = G
-- second one will be dead code 
-- (Haskell doesn't treat it as a mistake here and so will I)


module Parser 
  (loadProgram) where

import Types

import Language.Haskell.Parser
import Language.Haskell.Syntax
import Control.Monad (unless, foldM)
import qualified Data.Map as Map
import qualified Data.Set as Set

-- ==============================================================================
-- Functions for HsModule parsing
-- ==============================================================================

-- Takes a HsExp data constructor and parses it to Expr defined by us.
hsExpToExpr :: HsExp -> Either String Expr
-- An application
hsExpToExpr (HsApp left right) = do
  l <- hsExpToExpr left
  r <- hsExpToExpr right
  return (l :$ r)
-- Parenthesis can be safely ommited as it doesn't affect built AST structure.
hsExpToExpr (HsParen expr) = hsExpToExpr expr
-- A variable
hsExpToExpr (HsVar (UnQual (HsIdent x))) = Right (Var x) 
-- A constructor
hsExpToExpr (HsCon (UnQual (HsIdent x))) = Right (Con x)
-- Nothing else is allowed. 
hsExpToExpr _ = Left "Unsupported expression format. Only variables, constructors and applications are allowed."

-- Takes a hsPat which represents, in our case, a variable or constructor 
-- and returns a Pat (pattern) defined by us.
hsPatToPat :: HsPat -> Either String Pat
-- A variable
hsPatToPat (HsPVar (HsIdent pat)) = Right (PVar pat)
-- Parenthesis can be safely ommited as it doesn't affect built AST structure.
hsPatToPat (HsPParen pat) = hsPatToPat pat
-- A constructor (which can take arguments)
hsPatToPat (HsPApp (UnQual (HsIdent name)) args) = do
  parsedArgs <- mapM hsPatToPat args
  return (PApp name parsedArgs)
-- Nothing else is allowed.
hsPatToPat _ = Left "Unsupported pattern format."

-- Takes a single HsMatch data constructor and parses it to Match defined by us.
hsMatchToMatch :: HsMatch -> Either String Match
hsMatchToMatch (HsMatch _ (HsIdent name) patList (HsUnGuardedRhs expr) _) = do
  pats <- mapM hsPatToPat patList
  e <- hsExpToExpr expr
  return $ Match name pats e


-- Takes a HsDecl data constructor which, in our case, can be 
-- a HsFunBind for a function that takes arguments 
-- or HsPatBind for a one that doesn't
-- and returns a Def (definition) defined by us.
hsDeclToDef :: HsDecl -> Either String Def 
-- Format: function_name = expression  - a constant function which takes no arguments.
hsDeclToDef (HsPatBind _ (HsPVar (HsIdent name)) (HsUnGuardedRhs hsExp) _) = do
  expr <- hsExpToExpr hsExp
  return $ Def [Match name [] expr] 
-- Format: [function_name function_arguments = expression] - list of matches.
-- There can be multiple matches, just like in normal Haskell when we have pattern matching.
hsDeclToDef (HsFunBind matches) = do
  parsedMatches <- mapM hsMatchToMatch matches
  return $ Def parsedMatches
hsDeclToDef _ = 
  Left "Unknown declaration format."

-- ==============================================================================
-- Program validation
-- ==============================================================================

-- Ad concatMap: it is equivalent to concat . map, but more efficient, as GHC optimizes it.

-- Extracts a flat list of all argument names in the pattern.
extractVars :: Pat -> [Name]
extractVars (PVar n) = [n]
extractVars (PApp _ pats) = concatMap extractVars pats

-- Checks if the function's definition is correct.
checkMatch :: Match -> Either String ()
checkMatch (Match name pats _) = do
  -- Makes a flat list of all argument names in the function Match.
  let vars = concatMap extractVars pats
  unless (Set.size (Set.fromList vars) == length vars) $
    Left $ "Arguments of function '" ++ name ++ "' are not unique."

getLengthOfMatch :: Match -> Int
getLengthOfMatch (Match _ pats _) = length pats

allSame :: Eq a => [a] -> Bool
allSame []     = True
allSame (x:xs) = all (== x) xs

-- Checks if all of the function's definitions are correct and have the same arity.
-- (It seems like ParseModule catches it before this function anyways... lol)
checkDef :: Def -> Either String ()
checkDef (Def matches) = do 
  mapM_ checkMatch matches
  let lengths = map getLengthOfMatch matches
  if allSame lengths then Right () else case matches of
    (m:_) -> Left $ "Function '" 
                   ++ matchName m ++ "' has definitions of different arity."
    []    -> Right () -- THIS SHOULD NEVER HAPPEN ANYWAYS (added for completeness).

buildDefMap :: [Def] -> Either String DefMap
buildDefMap defs = foldM insertDef Map.empty defs
  where
    insertDef m d@(Def (match:_)) = 
      let name = matchName match 
      in if Map.member name m
         then Left $ "Duplicate definitions of function '" ++ name 
          ++ "'. All occurences must be next to each other!"
         else Right $ Map.insert name d m
    insertDef m (Def []) = Right m -- THIS SHOULD NEVER HAPPEN ANYWAYS (added for completeness).

-- ==============================================================================
-- Main pipeline
-- ==============================================================================

loadProgram :: FilePath -> IO (Either String (DefMap, Expr, [Def]))
loadProgram fileName = do
  -- Unpack the String from IO String monad returned by readFile.
  input <- readFile fileName
  -- First, we will evaluate our "Either" expression and then pack it into IO (...).
  return $ do
    hsModule <- case parseModule input of 
      ParseFailed loc err -> Left $ "Syntax error on line " ++ show loc ++ ": " ++ err
      ParseOk a           -> Right a  
    
    let HsModule _ _ _ _ declarations = hsModule

    -- mapM maps a function of type a -> m b over a list 
    -- and gives all the results as a m [b]. mapM_ does the same thing, 
    -- but never collects the results, returning a m ().

    -- Unless any of the declarations is incorrect, we will get 'Right [Def]'.
    -- Otherwise, 'Left' with the message describing the error will be propagated.
    definitions <- mapM hsDeclToDef declarations
    mapM_ checkDef definitions
    defMap <- buildDefMap definitions
    
    mainExpr <- case Map.lookup "main" defMap of
      Nothing -> Left "Abscence of 'main'."
      Just (Def matches) -> 
        case matches of
          (Match _ [] expr : _) -> Right expr
          _ -> Left "'main' cannot take parameters."
    
    return (defMap, mainExpr, definitions)