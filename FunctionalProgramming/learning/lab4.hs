data Nat  = Zero | S Nat
add m Zero = m 
add m (S n) = S(add m n)

mul m Zero = Zero 
mul m (S n) = add (mul m n) m 

pot m Zero = S Zero 
pot m (S n) = mul (pot m n) m

-- fi (a,_) = a
-- se (_,b) = b
-- dziel Zero n = (Zero,Zero) 
-- dziel (S m) n = if newSe < n then (lastFi,newSe) else (S lastFi, Zero)
--   where 
--     result = dziel m n
--     lastFi = fi result
--     newSe = S (se result)

dziel Zero n = (Zero, Zero)
dziel (S m) n = if newR == n then (S q, Zero) else (q, newR)
  where
    (q, r) = dziel m n  -- natychmiastowe rozpakowanie krotki
    newR = S r

instance Eq Nat where
  Zero == Zero = True
  S m  == S n = m == n 
  _    == _ = False 

instance Num Nat where
  (+) = add 
  (*) = mul 
  fromInteger 0 = Zero 
  fromInteger n = S(fromInteger(n-1))

instance Show Nat where
  showsPrec _ Zero = showString "Zero"
  showsPrec p (S n) = showParen (p > 10) (showString "S " . showsPrec 11 n)
  