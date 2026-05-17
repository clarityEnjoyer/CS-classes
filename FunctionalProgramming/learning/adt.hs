import Data.Map (Map)

--uczę się typów danych. 15.03.2026

--napiszmy klasyczny rekurencyjny algebraiczny

data Expr = Liczba Int | Dodaj Expr Expr | Mnoz Expr Expr | Dziel Expr Expr deriving (Show)

oblicz :: Expr -> Maybe Int
oblicz (Liczba a) = Just a
oblicz (Dodaj a b) 
  |Just a <- oblicz a, Just b <- oblicz b = Just $ a + b
  |otherwise                              = Nothing
oblicz (Mnoz a b)
  |Just a <- oblicz a, Just b <- oblicz b = Just $ a * b
  |otherwise                              = Nothing
oblicz (Dziel a b) 
  |Just a <- oblicz a, Just b <- oblicz b, b/=0 = Just $ a `div` b
  |otherwise                                    = Nothing

elimMaybe :: c -> (a -> c) -> Maybe a -> c
elimMaybe c ac a 
  |Just a <- a = ac a 
  |otherwise   = c

fromMaybe :: a -> Maybe a -> a
fromMaybe a a' 
  |Just a' <- a' = a' 
  |otherwise     = a

mapMaybe :: (a -> b) -> Maybe a -> Maybe b
mapMaybe ab a 
  |Just a <- a = Just (ab a)
  |otherwise   = Nothing

maybeHead :: [a] -> Maybe a
maybeHead [] = Nothing
maybeHead (x:xs) = Just x 

elimEither :: (a  -> c) -> (b -> c) -> Either a b -> c
elimEither ac bc (Left a) = ac a 
elimEither ac bc (Right b) = bc b 
--CIEKAWOSTKA
-- Zamiast konstruktorów danych, tworzymy funkcje:
-- myLeft :: a -> (a -> c) -> (b -> c) -> c
-- myLeft x = \f g -> f x

-- myRight :: b -> (a -> c) -> (b -> c) -> c
-- myRight y = \f g -> g y

-- elimEither :: (a -> c) -> (b -> c) -> ((a -> c) -> (b -> c) -> c) -> c
-- elimEither f g e = e f g
--podajesz dwie funkcje do obiektu e, a on sam "wie", 
--którą z nich wywołać na podstawie tego, 
--jak został skonstruowany (myLeft wywoła f, a myRight wywoła g).

mapEither :: (a1 -> a2) -> (b1 -> b2) -> Either a1 b1 -> Either a2 b2
mapEither a b (Left x) = Left (a x)
mapEither a b (Right x) = Right (b x)

fromEither :: Either a a -> a
fromEither (Left a) = a 
fromEither (Right a) = a 

hmanyDivisors :: Int -> Int 
hmanyDivisors n = sum $ [1 | x <- [1,2..n], n `mod` x == 0]

--mam puste pudełko albo trzy przegrodki: na wartosc i dwa inne drzewa
data Tree a = Empty | Node a (Tree a) (Tree a) deriving (Eq, Ord, Show)

fullTree :: Int -> Tree Int 
fullTree 0 = Empty
fullTree n = Node rootVal lewe prawe
  where rootVal = 2^(n-1)
        lewe    = fullTree $ n-1 
        prawe   = tMap (+rootVal) lewe
    
tMap :: (Int -> Int) -> Tree Int -> Tree Int
tMap f Empty = Empty
tMap f (Node a left right) = Node (f a) (tMap f left) (tMap f right)

--w porzadku inorder:)
toList :: Tree a -> [a]
toList Empty = []
toList (Node a left right) = toList left ++ ([a] ++ toList right)