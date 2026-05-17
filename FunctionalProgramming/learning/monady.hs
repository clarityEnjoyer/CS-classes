data Exp = IntE Int
        | OpE Op Exp Exp
        | VarE String
        | LetE String Exp Exp -- let var = e1 in e2
        -- mam zrobic interpretuj od leta. oblicz wartosc wyrazenia let . 
        --Wrzucic do globalnego stanu, wykonac i wyczyscic tę informację

Type Op = Int -> Int -> Int

data Stmt = S --skip
        | AS String Exp -- x := e 
        | SeqS Stmt Stmt -- sekwencja S1; S2
        | IfS Expr Stmt Stmt 
        | WhileS Expr Stmt

type Mem  = M.Map String Int 
type MyMonad a = State Mem a

interpretuj
wykonaj 
