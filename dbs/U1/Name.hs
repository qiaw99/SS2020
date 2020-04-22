main = do 
    putStrLn "Hello, what's your name?"
    name <- getLine
    putStrLn ("Hallo " ++ name ++ "!")
