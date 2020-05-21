{-# LANGUAGE OverloadedStrings #-}

module Main where

import Database.PostgreSQL.Simple

main :: IO ()
main = do
  -- connection to db
  conn <- connect defaultConnectInfo {
      connectUser = "postgres",
      connectPassword = "qianli99",
      connectDatabase = "postgres"
    }

  -- execute query
  result <- query_ conn "SELECT NAME FROM COMPANY"
  -- unpack the data
  let names = map fromOnly (result :: [Only String])
  print names
  -- close the connection
  close conn
