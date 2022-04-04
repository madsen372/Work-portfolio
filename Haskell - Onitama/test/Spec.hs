import qualified Lib
import System.Directory
import System.FilePath.Posix
import qualified Data.List
import qualified Data.Set
import qualified Data.Text
import Control.Monad

-- strip newlines and white spaces from a string
strip :: String -> String
strip = Data.Text.unpack . Data.Text.strip . Data.Text.pack

-- checks if the first string contains at least one substring passed in the second argument
hasSubstring :: String -> [String] -> Bool
hasSubstring x xs = or (map (Data.Text.isInfixOf (Data.Text.pack x)) (map Data.Text.pack xs))

-- retrieve all the basename in a directory and append the dir
getAllBaseNames :: String -> IO ([String])
getAllBaseNames dir = liftM (map (dir </>) . Data.Set.toList . Data.Set.fromList . map takeBaseName) (listDirectory dir)

-- given a file name and two strings, check if the strings are equal and if not raise an error
check :: String -> String -> String ->  IO ()
check f s1 s2 = if (strip s1) == (strip s2)
 then
    return ()
  else
    ioError $ userError $ "Failure of test " ++ f ++ ": Expected " ++ (show s2) ++ ", obtained instead " ++ (show s1)


applyGenerateRandom :: String -> IO ()
applyGenerateRandom f = do
  [seed, maxDepth] <- liftM ((map read) . lines . strip) (readFile (f ++ ".param"))
  s1 <- Lib.generateRandom seed maxDepth
  s2 <- readFile (f ++ ".out")
  check f s1 s2
  s1 <- Lib.isValid (f ++ ".out")
  if hasSubstring s1 ["NotValid", "ParsingError"]
    then
      ioError $ userError $ "Failure of test " ++ f ++ ": The randomly generated moves are not valid."
    else
      return ()


applyIsValid :: String -> IO ()
applyIsValid f = do
  s1 <- Lib.isValid (f ++ ".in")
  s2 <- readFile (f ++ ".out")
  check f s1 s2

applyHasWinningStrategy :: String -> IO ()
applyHasWinningStrategy f = do
  maxDepth <- liftM (read . strip) (readFile (f ++ ".param"))
  s1 <- Lib.hasWinningStrategy maxDepth (f ++ ".in")
  s2 <- readFile (f ++ ".out")
  check f s1 s2

main :: IO ()
main = do
    projDir <- getCurrentDirectory
    tests <- getAllBaseNames (projDir </> "unit_tests" </> "generateRandom")
    putStrLn $ "the test on generateRandom are " ++ show tests
    mapM_ applyGenerateRandom tests

    tests <- getAllBaseNames (projDir </> "unit_tests" </> "isValid")
    putStrLn $ "the test on isValid are " ++ show tests
    mapM_ applyIsValid tests

    tests <- getAllBaseNames (projDir </> "unit_tests" </> "hasWinningStrategy")
    putStrLn $ "the test on hasWinningStrategy are " ++ show tests
    mapM_ applyHasWinningStrategy tests

    putStrLn "Tests passed successfully"
