module Types where

import Control.Applicative (Alternative(..))
import Data.List (intercalate)
import qualified Data.Map as Map

type Name = String

infixl 9 :$
data Expr = Var Name | Con Name | Expr :$ Expr | Focus Expr deriving (Eq)
data Pat  = PVar Name | PApp Name [Pat] deriving (Eq)
data Match = Match { matchName :: Name, matchPats :: [Pat], matchRhs :: Expr }
data Def = Def { defMatches :: [Match] }
type DefMap = Map.Map Name Def 



-- SHOW INSTANCES:
-- ==============================================================================
-- 1. Instance for Expr (expressions)
-- ==============================================================================
instance Show Expr where
  showsPrec _ (Var name) = showString name
  showsPrec _ (Con name) = showString name
  showsPrec _ (Focus e)  = showString "{" . showsPrec 0 e . showString "}"
  showsPrec p (left :$ right) = 
    showParen (p > 10) (showsPrec 10 left . showString " " . showsPrec 11 right)
-- ==============================================================================
-- 2. Instance for Pat (patterns)
-- ==============================================================================
instance Show Pat where
  showsPrec _ (PVar name) = showString name
  
  -- A constructor without arguments doesn't require parenthesis.
  showsPrec _ (PApp name []) = showString name
  
  -- A constructor with arguments requires parenthesis depending on the context.
  showsPrec p (PApp name args) = 
    showParen (p > 10) $ 
      showString name . showArgs args
    where
      -- foldr adds a space and an argument. 
      showArgs = foldr (\arg acc -> showChar ' ' . showsPrec 11 arg . acc) id

-- ==============================================================================
-- 3. Instance for Match
-- ==============================================================================
instance Show Match where
  showsPrec _ (Match name pats expr) = 
    showString name . showArgs pats . showString " = " . showsPrec 10 expr
    where 
      showArgs = foldr (\pat acc -> showChar ' ' . showsPrec 11 pat . acc) id

-- ==============================================================================
-- 4. Instance for Def
-- ==============================================================================
instance Show Def where
  -- Definition is a list of matches, so we just show them and separate with newline.
  show (Def matches) = intercalate "\n" (map show matches)


-- ==============================================================================
-- SnocList - A sequence type with O(1) insertion at the end. 
-- Logically it is a normal list; physically it's internal list is reversed.
-- ==============================================================================
newtype SnocList a = SnocList { unSnocList :: [a] }

-- toList returns a physical List which behaves like logical SnocList,
-- by reversing SnocList back to normal order. Time O(n).
toList :: SnocList a -> [a]
toList (SnocList xs) = reverse xs

-- Builds SnocList from a List, by reversing it. Time O(n).
fromList :: [a] -> SnocList a
fromList xs = SnocList (reverse xs)

-- Logical insertion at the end of the SnocList
-- (i.e. physical insertion at the begging of the reversed list). Time O(1).
snoc :: SnocList a -> a -> SnocList a
snoc (SnocList xs) x = SnocList (x : xs)

instance Show a => Show (SnocList a) where
    show sl = "SnocList " ++ show (toList sl)

instance Eq a => Eq (SnocList a) where
    sl1 == sl2 = toList sl1 == toList sl2

instance Semigroup (SnocList a) where
    -- We have xs = reversed xs', ys = reversed ys' and want to get reversed (xs' ++ ys'),
    -- which is (reversed ys' ++ reversed xs') = (ys ++ xs) 
    SnocList xs <> SnocList ys = SnocList (ys ++ xs)

instance Monoid (SnocList a) where
    mempty = SnocList []

instance Functor SnocList where
    fmap f (SnocList xs) = SnocList (map f xs)

instance Applicative SnocList where
    pure x = SnocList [x]
    slF <*> slX = fromList $ toList slF <*> toList slX

instance Alternative SnocList where
    empty = mempty
    -- By treating a list as a non-deterministic computation (i.e. many possible outcomes), 
    -- we easily see that alternative of two lists is just their concatenation.
    (<|>) = (<>)