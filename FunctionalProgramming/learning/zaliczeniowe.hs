--Aliaksei Papkouski 484417 
--15.03.2026 Programowanie Funkcyjne

data Expr = S | K | I | B 
          | Expr :$ Expr 
          | X | Z | V Int  
          deriving (Show, Read)

test1 = S :$ K :$ K :$ X
twoB = S :$B :$ I
threeB = S :$ B :$ (S :$B :$ I)
test3 = threeB :$ X :$ Z
omega = ((S :$ I) :$ I) :$ ((S :$ I) :$ I)
kio = K :$ I :$ omega
add = (B :$ S) :$ (B :$ B)

prettyExpr :: Expr -> String 
prettyExpr S = "S"
prettyExpr K = "K"
prettyExpr I = "I"
prettyExpr B = "B"
prettyExpr X = "x"
prettyExpr Z = "z"
prettyExpr (V n) = "v" ++ show n
prettyExpr (a :$ b) = prettyExpr a ++ " " ++ prawy
  where prawy = wrapIfNeeded b
        wrapIfNeeded (c :$ d) = "(" ++ prettyExpr (c :$ d) ++ ")"
        wrapIfNeeded x = prettyExpr x 
        

rstep :: Expr -> Maybe Expr 
rstep (I :$ x) = Just x
rstep (K :$ x :$ y) = Just x
rstep (S :$ x :$ y :$ z) = Just ((x :$ z) :$ (y :$ z))
rstep (B :$ x :$ y :$ z) = Just (x :$ (y :$ z))
rstep (e1 :$ e2)
  |Just re1 <- rstep e1 = Just (re1 :$ e2) 
  |Just re2 <- rstep e2 = Just (e1 :$ re2)
  |otherwise            = Nothing
rstep _ = Nothing

rpath :: Expr -> [Expr]
rpath e = e : case rstep e of 
                Nothing -> []
                Just e' -> rpath e'

-- o mapM_ do wypisywania dopytałem się AI Gemini
printPath e = mapM_ putStrLn $ take 30 $ map prettyExpr (rpath e)

--funkcja ktora bedzie szukac I i K do skrocenia w CALYM drzewie
shrink :: Expr -> Maybe Expr
shrink (I :$ x) = Just x
shrink (K :$ x :$ y) = Just x
shrink (e1 :$ e2) 
  | Just re1 <- shrink e1 = Just (re1 :$ e2)
  | Just re2 <- shrink e2 = Just (e1 :$ re2)
  | otherwise             = Nothing
shrink _ = Nothing

rstep' :: Expr -> Maybe Expr
rstep' e = case shrink e of
             Just e' -> Just e'
             Nothing -> rstep e

rpath' :: Expr -> [Expr]
rpath' e = e : case rstep' e of 
                Nothing -> []
                Just e' -> rpath' e'

printPath' e = mapM_ putStrLn $ take 30 $ map prettyExpr $ rpath' e