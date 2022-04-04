module Lib
  ( generateRandom
  , isValid
  , hasWinningStrategy
  )
where

import           System.IO
import           Control.Monad
import           System.Random
import           Data.List
import           System.Random.Shuffle
import           Data.IORef
import           Text.Read
import           Debug.Trace
import           Data.Maybe
import qualified Data.Set                      as Set

data PlayerType = Player1 | Player2
  deriving (Eq)

data Card = Cobra
          | Monkey
          | Rabbit
          | Rooster
          | Tiger
  deriving (Eq, Ord, Show)

allCards = [Cobra, Monkey, Rabbit, Rooster, Tiger]
allCardNames = fmap show allCards

cardFromStr :: String -> Card
cardFromStr "Cobra"   = Cobra
cardFromStr "Monkey"  = Monkey
cardFromStr "Rabbit"  = Rabbit
cardFromStr "Rooster" = Rooster
cardFromStr "Tiger"   = Tiger


cardMoves :: Card -> [(Int, Int)]
cardMoves Cobra   = [(0, -1), (1, 1), (-1, 1)]
cardMoves Monkey  = [(1, -1), (1, 1), (-1, 1), (-1, -1)]
cardMoves Rabbit  = [(-1, -1), (1, 1), (0, 2)]
cardMoves Rooster = [(0, -1), (-1, -1), (0, 1), (1, 1)]
cardMoves Tiger   = [(-1, 0), (2, 0)]

cardMovesPlayer :: (Int, Int) -> Player -> Card -> [(Int, Int)]
cardMovesPlayer (x, y) player card =
  let d     = if pType player == Player1 then 1 else -1
      moves = fmap (\(a, b) -> (x + d * a, y + d * b)) (cardMoves card)
  in  filter (isLegalLocation player) moves


type Location = (Int, Int)

isLegalLocation :: Player -> Location -> Bool
isLegalLocation player (x, y) =
  x
    >= 0
    && y
    >= 0
    && x
    <= 4
    && y
    <= 4
    && (not $ elem (x, y) (pLocations player))

data Player = Player { pLocations :: [(Int, Int)]
                     , pCards :: [Card]
                     , pType :: PlayerType
                     }

player1StartLocations = [(0, 2), (0, 0), (0, 1), (0, 3), (0, 4)]
player2StartLocations = [(4, 2), (4, 0), (4, 1), (4, 3), (4, 4)]

playerCapture :: (Int, Int) -> Player -> Player
playerCapture (x, y) player
  | pLocations player == []              = player
  | (head $ pLocations player) == (x, y) = player { pLocations = [] }
  | pType player == Player1 && (x, y) == (0, 2) = player { pLocations = [] }
  | pType player == Player2 && (x, y) == (4, 2) = player { pLocations = [] }
  | otherwise = player { pLocations = delete (x, y) (pLocations player) }

playerMove :: (Int, Int) -> (Int, Int) -> Player -> Player
playerMove (x, y) (x', y') player = player
  { pLocations = fmap replace (pLocations player)
  }
 where
  replace (a, b) | (a, b) == (x, y) = (x', y')
                 | otherwise        = (a, b)

playerSwitchCard :: Card -> Card -> Player -> Player
playerSwitchCard card card' player =
  let cards = card' : (delete card (pCards player))
  in  player { pCards = cards }

data Game = Game { gPlayer1 :: Player
                 , gPlayer2 :: Player
                 , gSideCard :: Card
                 , gPlaying :: PlayerType
                 }

isGameFinished :: Game -> Bool
isGameFinished (Game player1 player2 _ _) =
  pLocations player1 == [] || pLocations player1 == []

getPlayer :: Game -> Player
getPlayer game | gPlaying game == Player1 = gPlayer1 game
               | otherwise                = gPlayer2 game


instance Show Game where
  show (Game player1 player2 card _) =
    show
      $ ( fmap show $ (pCards player1) ++ (pCards player2) ++ [card]
        , pLocations player1
        , pLocations player2
        )

data Move = Move (Int,Int) (Int,Int) Card

instance Show Move where
  show (Move xy xy' c) = show (xy, xy', (show c))

randomCards :: IO [Card]
randomCards = do
  g <- newStdGen
  return $ shuffle' allCards (length allCards) g

selectCard :: Player -> IO Card
selectCard (Player _ cards _) = do
  index <- randomIO
  return $ cards !! (index `mod` 2)

selectLocation :: Player -> IO Location
selectLocation (Player locations _ _) = do
  index <- randomIO
  return $ locations !! (index `mod` (length locations))

selectCardMove :: Player -> (Int, Int) -> Card -> IO (Maybe (Int, Int))
selectCardMove player xy card = do
  index <- randomIO
  let moves = cardMovesPlayer xy player card
  if moves == []
    then return Nothing
    else do
      return $ Just $ moves !! (index `mod` (length moves))

gameMove :: Game -> (Int, Int) -> (Int, Int) -> Card -> Game
gameMove (Game player1 player2 sideCard playing) xy xy' card =
  let player1' = if playing == Player1
        then playerSwitchCard card sideCard $ playerMove xy xy' player1
        else playerCapture xy' player1
      player2' = if playing == Player1
        then playerCapture xy' player2
        else playerSwitchCard card sideCard $ playerMove xy xy' player2
  in  Game player1'
           player2'
           card
           (if playing == Player1 then Player2 else Player1)

randomMove :: Game -> IO (Move, Game)
randomMove game = do
  let player = (getPlayer game)
  card <- selectCard player
  xy   <- selectLocation player
  ret  <- selectCardMove player xy card
  case ret of
    Just xy' -> do
      let game' = gameMove game xy xy' card
      return ((Move xy xy' card), game')
    Nothing -> randomMove game

generateRandom :: Int -> Int -> IO (String)
generateRandom seed n = do
  setStdGen $ mkStdGen seed
  [a, b, c, d, e] <- sort <$> randomCards
  let sideCard = a
      player1  = Player player1StartLocations (sort [b, c]) Player1
      player2  = Player player2StartLocations (sort [d, e]) Player2
      game     = Game player1 player2 sideCard Player1

  theGame <- newIORef game
  moves   <- forM [1 .. n] $ \i -> do
    game <- readIORef theGame
    if isGameFinished game
      then do
        return Nothing
      else do
        (move, game) <- randomMove game
        writeIORef theGame game
        return $ Just move
  return $ intercalate "\n" $ [show game] ++ (fmap show (catMaybes moves))

isValid :: FilePath -> IO (String)
isValid filePath = do
  fileLines <- fmap lines $ readFile filePath
  if length fileLines < 1
    then return "ParsingError"
    else do
      let (gameStr : movesStr) = fileLines
      case parseGame gameStr of
        Just game -> case parseMoves movesStr of
          Just moves -> do
            case checkGame game moves of
              Right m -> return $ "NonValid " ++ (show m)
              Left  g -> return $ show g
          Nothing -> return "ParsingError"
        Nothing -> return "ParsingError"

parseGame :: String -> Maybe Game
parseGame line =
  case readMaybe line :: Maybe ([String], [(Int, Int)], [(Int, Int)]) of
    Just (cards, player1Locations, player2Locations) ->
      if (length (Set.fromList cards) /= 5)
           || (player1Locations /= player1StartLocations)
           || (player2Locations /= player2StartLocations)
           || any (\x -> not (elem x allCardNames)) cards
        then Nothing
        else
          let sideCard     = head $ fmap cardFromStr $ take 1 $ drop 4 cards
              player1Cards = fmap cardFromStr $ take 2 cards
              player2Cards = fmap cardFromStr $ take 2 $ drop 2 cards
          in  Just $ Game (Player player1Locations player1Cards Player1)
                          (Player player2Locations player2Cards Player2)
                          sideCard
                          Player1
    Nothing -> Nothing

parseMoves :: [String] -> Maybe [Move]
parseMoves lines =
  let moves = fmap parseMove lines
  in  if any isNothing moves then Nothing else Just $ catMaybes moves

parseMove :: String -> Maybe Move
parseMove line =
  case readMaybe line :: Maybe ((Int, Int), (Int, Int), String) of
    Just (xy, xy', cardStr) -> Just $ Move xy xy' (cardFromStr cardStr)
    Nothing                 -> Nothing

checkGame :: Game -> [Move] -> Either Game Move
checkGame game []                          = Left game
checkGame game (m@(Move xy xy' card) : ms) = if checkGameMove game m
  then let game' = gameMove game xy xy' card in checkGame game' ms
  else Right m

checkGameMove :: Game -> Move -> Bool
checkGameMove (Game player1 player2 _ playing) (Move xy xy' card) =
  let player = if playing == Player1 then player1 else player2
  in  checkPlayer player
 where
  checkPlayer player | not $ elem xy (pLocations player1) = False
                     | not $ isLegalLocation player xy'   = False
                     | not $ elem xy' (cardMovesPlayer xy player card) = False
                     | otherwise                          = True

hasWinningStrategy :: Int -> FilePath -> IO (String)
hasWinningStrategy _ _ = return "Not yet implemented"
