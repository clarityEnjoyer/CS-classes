import Data.Array (listArray)
-- I can skip x's due to eta reduction!
zero f x = x
one f = f

suc n f = f $ n f
add n m f = n f $ m f
mul n m f = n $ m f
--function to convert church numbers to natural 
tint n = n (+1) 0
