-- :(( pokonało mnie to, ale wrócę silniejszy... (za parę tyg. to zrobię)
-- nie wiem co dokładnie (bo brak mi trochę wiedzy i czasu), ale przy testowaniu dostaję niepoprawne wyniki (chyba)
-- inspirowalem sie https://ssojet.com/data-structures/implement-segment-tree-in-haskell#data-structure-definition-and-initialization
-- ciekawskich odsyłam do https://github.com/sourabhxyz/haskell-competitive-programming/tree/main/segment-trees 
-- tam powinno byc dobrze:)

data Segtree a = Leaf a | Node a a (Segtree a) (Segtree a)
  deriving (Show)

-- buildTree :: (a -> a -> a) -> [a] -> Segtree a
buildTree combine [x] = Leaf x
buildTree combine xs = Node (combine leftVal rightVal) 0 leftTree rightTree
  where
    mid = length xs `div` 2
    (left, right) = splitAt mid xs
    leftTree = buildTree combine left
    rightTree = buildTree combine right
    leftVal = case leftTree of Leaf v -> v; Node v _ _ _ -> v
    --I ingore lazy val so far, cuz it shouldnt be needed when building
    rightVal = case rightTree of Leaf v -> v; Node v _  _ _ -> v

propagate :: Num a => Segtree a -> Segtree a
propagate (Leaf v) = Leaf v
propagate (Node treeVal lazyVal left right) = Node treeVal lazyVal' left' right'
  where
    lazyVal' = 0
    left' = case left of
      Leaf v -> Leaf (v + lazyVal)
      Node v lv l r -> Node (v + lazyVal) (lv + lazyVal) l r
    right' = case right of
      Leaf v -> Leaf (v + lazyVal)
      Node v lv l r -> Node (v + lazyVal) (lv + lazyVal) l r

identity :: Int
identity = -1000000000

-- AKSING FOR A MAX  
query combine tree = query' combine (propagate tree)
query' combine (Leaf val) _ _ _ _ = val
query' combine (Node treeVal lazyVal left right) treeLow treeHigh queryLow queryHigh
  | queryLow <= treeLow && treeHigh <= queryHigh = treeVal -- Node fully within query range
  | treeHigh < queryLow || queryHigh < treeLow = identity
  | otherwise = combine (query combine left treeLow mid queryLow queryHigh)
                        (query combine right (mid + 1) treeHigh queryLow queryHigh)
  where mid = (treeHigh + treeLow) `div` 2


modify val combine tree = modify' val combine (propagate tree)
modify' val combine (Leaf v) _ _ _ _ = Leaf (v + val)
modify' val combine (Node treeVal lazyVal left right) treeLow treeHigh queryLow queryHigh
  | queryLow <= treeLow && treeHigh <= queryHigh = Node (treeVal + val) (lazyVal + val) left right -- Node fully within query range
  | treeHigh < queryLow || queryHigh < treeLow = Node treeVal lazyVal left right
  | otherwise = Node (combine leftVal rightVal) lazyVal' left' right'
  where
    mid = (treeHigh + treeLow) `div` 2
    lazyVal' = 0
    left' = modify val combine left treeLow mid queryLow queryHigh
    right' = modify val combine right (mid + 1) treeHigh queryLow queryHigh
    leftVal = case left' of Leaf v -> v; Node v _ _ _ -> v
    rightVal = case right' of Leaf v -> v; Node v _ _ _ -> v

