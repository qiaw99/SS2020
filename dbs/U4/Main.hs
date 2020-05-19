{-# LANGUAGE OverloadedStrings #-}

module Main where

import Database.PostgreSQL.Simple

main :: IO ()
main = do
  -- connection to db
  conn <- connect defaultConnectInfo {
      connectUser = "testuser",
      connectPassword = "password",
      connectDatabase = "testdb"
    }

  -- execute query
  result <- query_ conn "SELECT name FROM people"
  -- unpack the data
  let names = map fromOnly (result :: [Only String])
  print names
  -- close the connection
  close conn
