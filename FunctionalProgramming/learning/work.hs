--otóż okazuje się, że standardowy head nie jest bezpieczny XD :: [a] -> a (bez Maybe)
evenHeadChecker lista
  |Just x <- head' lista = even x
  |Nothing <- head' lista = False 

-- podniesOcene uczen studenci
--   | Just ocena <- Map.lookup uczen studenci = ocena + 5
--   | otherwise = 0

-- złe podejscie bo Zapis (x:xs) to nie jest wyrażenie logiczne – to jest wzorzec strukturalny. 
--Nie można go użyć gołego w miejscu, gdzie Haskell oczekuje pytania "prawda czy fałsz?".
-- head' lista
--   | null lista = Nothing
--   | (x:xs) = Just x

head' [] = Nothing
head' (x:_) = Just x

--name & surname to tylko napisy. napis pusty tez jest ok
firstLettersExctractor name surname 
  |Just firstName <- head' name, Just firstSurname <- head' surname = Just [firstName,firstSurname]
  |otherwise = Nothing

--fajny trikuś
--map ($ 3) [(4+), (10*), (^2), sqrt]  
--[7.0,30.0,9.0,1.7320508075688772]  

-- fn x = ceiling (negate (tan (cos (max 50 x))))  
-- fn = ceiling . negate . tan . cos . max 50  

-- oddSquareSum :: Integer  
-- oddSquareSum = sum (takeWhile (<10000) (filter odd (map (^2) [1..])))  
-- oddSquareSum = sum . takeWhile (<10000) . filter odd . map (^2) $ [1..]  
-- oddSquareSum = 
--     let oddSquares = filter odd $ map (^2) [1..]  
--         belowLimit = takeWhile (<10000) oddSquares  
--     in  sum belowLimit  

data Car = Car {year :: Int, model :: String} deriving (Show)
data List = Empty | X