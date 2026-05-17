-- data Tree a = Empty | Node a (Tree a) (Tree a)

-- instance Show a => Show (Tree a) where
--   show t = case t of
--     Empty -> "<>"
--     Node a l r -> "(Node " ++ show a ++ " " ++ show l ++ " " ++ show r ++ ")"

data Exp
    = EInt Int             -- stała całkowita
    | EAdd Exp Exp         -- e1 + e2
    | EMul Exp Exp         -- e1 * e2
    | EVar String          -- zmienna
    | ELet String Exp Exp  -- let var = e1 in e2
--  deriving Show
instance Show Exp where
  showsPrec p (EInt n) = showsPrec p n
  showsPrec p (EVar v) = showString v
  showsPrec p (EAdd e1 e2) = showParen (p > 6) $
    showsPrec 6 e1 . showString " + " . showsPrec 7 e2
  showsPrec p (EMul e1 e2) = showParen (p > 7) $
    showsPrec 7 e1 . showString " * " . showsPrec 8 e2
  showsPrec p (ELet v e1 e2) = showParen (p > 0) $
    showString "let " . showString v . showString " = " . showsPrec 0 e1 . showString " in " . showsPrec 0 e2


instance Num Exp where
  (-) a b = EAdd a (-b)
  (*) = EMul  
  (+) = EAdd
  fromInteger x = EInt (fromIntegral x)
  negate x = EMul x (-1)


