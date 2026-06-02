module Main where

import System.Environment (getArgs)
import Evaluator
import Parser

usageMsg :: String
usageMsg = "Usage: zadanie3 [--help] [file]\n\
           \  --help    - display this message\n\
           \  -- file   - file with program to reduce (note the space after --!)"
main :: IO ()
main = do 
  args <- getArgs 
  case args of
    -- []         -> putStrLn usageMsg
    ["--help"] -> putStrLn usageMsg
    [fileName] -> runEngine fileName
    _          -> putStrLn usageMsg

runEngine :: FilePath -> IO ()
runEngine fileName = do
  -- Parse the input file.
  result <- loadProgram fileName
  
  case result of
    Left err -> error err -- crash the program if there was an error!
    Right (defMap, expression, definitions) -> do
      
      -- 2. Debug: Wypisanie definicji
      -- (Zakładam, że dodasz instancję Show dla Def / Match, żeby to działało ładnie)
      mapM_ print definitions
      putStrLn "------------------------------------------------------------"
      
      -- 3. Inicjalizacja pierwszej linii outputu
      print expression
      
      -- 4. Główna ewaluacja
      -- Ustawiamy paliwo na np. 30 kroków, żeby nie zawiesić się w nieskończoność
      let history = generateHistory defMap expression 30 []
      
      -- 5. Wydruk historii kroków
      mapM_ print history