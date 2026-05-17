{-
technika pracy i sposób wykonania: 
probowalem wiekszosc robic i wymyslac sam. 
jezeli gdzies utknąłem, to pozwoliłem sobie się poradzić
znajomych (jak się najlepiej wczytac w dokumentację i sparsować drzewko src)
oraz AI (jak zrobic ten customowy pattern-match?? a no przeciez można spłaszczyć drzewo...
 - moja funkcja flatten - i potem juz latwo dopasowac argumenty itd:) )

komentarzy jest niewiele, gdyz kod raczej sam się wyjaśnia...

PS uzyłem error (proszę wybaczyć, wiem że brzydko...) gdyż i tak program musi się scrashować w przypadku błędu.
   wobec tego Either wydawało się przesadne (+ nie miałem czasu)
   tak samo zdaję sobie sprawę, że rozbicie na moduły można byłoby wykonać. niestety nie zdążyłem
-}

module Main where

import System.Environment (getArgs)
import Language.Haskell.Parser
import Language.Haskell.Syntax
import qualified Data.Map as Map
import qualified Data.Set as Set
import Control.Monad (unless)

data Def = Def Name [Pat] Expr
data Expr = Var Name | Expr :$ Expr
type Pat = Name
type Name = String

instance Show Def where
  showsPrec _ (Def name patList expr) = showString name . showArgs patList . showString " = " . showsPrec 10 expr
    where 
    showArgs :: [Pat] -> ShowS
    showArgs = foldr (\el acc -> showChar ' ' . showString el . acc) (showString "")

instance Show Expr where
  showsPrec _ (Var name) = showString name
  showsPrec p (left :$ right) = showParen(p>10) (showsPrec 10 left . showString " " . showsPrec 11 right)

newtype Prog = Prog {progDefs :: [Def]}
type DefMap = Map.Map Name Def 

buildDefMap :: Prog -> DefMap
buildDefMap (Prog defs) = Map.fromList $ zip (map getNameOfDef defs) defs
  where getNameOfDef (Def name _ _) = name

checkProg :: Prog -> Bool
checkProg (Prog defs) = foldr (\def acc -> isCorrectDef def && acc) True defs
  where 
    isCorrectDef (Def _ patList _) = allUnique patList
    allUnique l = Set.size (Set.fromList l) == length l

listOfDeclFromHsModule :: HsModule -> [HsDecl]
listOfDeclFromHsModule (HsModule _ _ _ _ a) = a

hsExpToExpr :: HsExp -> Expr
hsExpToExpr (HsApp left right) = hsExpToExpr left :$ hsExpToExpr right
hsExpToExpr (HsParen expr)     = hsExpToExpr expr
hsExpToExpr (HsVar (UnQual (HsIdent x))) = Var x 
hsExpToExpr (HsCon (UnQual (HsIdent x))) = Var x 
hsExpToExpr _ = error "\nNieobsługiwany format prawej strony kombinatora"

unboxPat :: HsPat -> Pat
unboxPat (HsPVar (HsIdent pat)) = pat
unboxPat _ = error "\nNieobsługiwany format argumentów.\n Użyj prostszej/czystszej składni!"

hsDeclToDef :: HsDecl -> Def 
hsDeclToDef (HsPatBind _ (HsPVar (HsIdent name)) (HsUnGuardedRhs expr) _) =
  Def name [] (hsExpToExpr expr) 
hsDeclToDef (HsFunBind [HsMatch _ (HsIdent name) patList (HsUnGuardedRhs expr) _]) =
  Def name (map unboxPat patList) (hsExpToExpr expr) 
hsDeclToDef (HsFunBind (_:_:_)) =
  error "Powtórzone definicje kombinatorów."
hsDeclToDef _ = error "\nNieobsługiwany format wyrażenia.\n Użyj prostszej/czystszej składni!"

flatten :: Expr -> [Expr] -> [Expr]
flatten (Var s) list = Var s : list
flatten (left :$ right) list = flatten left (right : list)

reducing :: [Expr] -> DefMap -> Maybe ([Pat], [Expr], Expr)
reducing (Var name : args) defMap = 
  case Map.lookup name defMap of 
    Nothing -> Nothing 
    Just (Def _ patList expr) -> Just (patList, args, expr)
reducing _ _ = Nothing

rstep :: DefMap -> Expr -> Maybe Expr
rstep defMap expr = 
  case tryReduce defMap expr of 
    Just expr' -> Just expr'                    
    Nothing    -> case expr of                  
                    Var _ -> Nothing            
                    left :$ right ->
                      case rstep defMap left of 
                        Just left' -> Just (left' :$ right)
                        Nothing    -> case rstep defMap right of 
                                        Just right' -> Just (left :$ right')
                                        Nothing     -> Nothing

rpath :: DefMap -> Expr -> [Expr]
rpath defMap e = e : case rstep defMap e of 
                Nothing -> []
                Just e' -> rpath defMap e'

tryReduce :: DefMap -> Expr -> Maybe Expr 
tryReduce defMap expr =
  case reducing (flatten expr []) defMap of
    Just (patList, args, body) 
      | length args >= length patList -> 
          let 
              env = zip patList args
              newRoot = subst env body
              remainingArgs = drop (length patList) args
              rebuiltExpr = foldl (\acc x -> acc :$ x) newRoot remainingArgs
          in Just rebuiltExpr
    _ -> Nothing

subst :: [(Name, Expr)] -> Expr -> Expr
subst env (Var name) = 
  case lookup name env of
    Just newExpr -> newExpr   
    Nothing      -> Var name    
subst env (left :$ right) = 
  subst env left :$ subst env right

usageMsg :: String
usageMsg = "Usage: zadanie2 [--help] [file]\n  --help  - display this message\n  file    - file with program to reduce"

main :: IO ()
main = do 
  args <- getArgs 
  case args of
    []         -> putStrLn usageMsg
    ["--help"] -> putStrLn usageMsg
    [fileName] -> runEngine fileName
    _          -> putStrLn usageMsg

runEngine :: FilePath -> IO ()
runEngine fileName = do
  input <- readFile fileName
  let parseResult = parseModule input
  let hsModule = case parseResult of 
                    ParseFailed _ _ -> error "\nPodany zestaw kombinatorów używa niedozwolonej w tym zadaniu składni."
                    ParseOk a       -> a  
  
  let declarations = listOfDeclFromHsModule hsModule
  let definitions  = map hsDeclToDef declarations
  let programme    = Prog definitions 
  let defMap       = buildDefMap programme
  
  unless (checkProg programme) $ 
    error "Argumenty kombinatora nie są unikatowe!"

  unless (Map.size defMap == length definitions) $ 
    error "Powtórzone definicje kombinatorów."

  let expression = case Map.lookup "main" defMap of
        Nothing                -> error "Brak wyrażenia \"main\"!"
        Just (Def _ pats expr) -> 
          if null pats
            then expr
            else error "Main nie powinien mieć argumentów."
  
  mapM_ print definitions 
  putStrLn "------------------------------------------------------------"
  
  mapM_ print $ take 30 (rpath defMap expression)
