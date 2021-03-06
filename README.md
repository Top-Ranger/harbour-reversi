A reversi (also known as Othello) implementation for SailfishOS

This implementation features an easy way to implement different players.
Currently avaiable are:
   * Human: A human player
   * Random AI: A simple AI selecting a random tile
   * Greedy AI: Tries to get as many points as possible
   * Tree AI: Builds a tree and tries to get the best possible result. Sometimes gets greedy.
   * Balanced AI: An AI which tries to combine the more clever aspect of the "Tree AI" with the power of the "Greedy AI"
   * Static Rule AI: An AI which determines the best turn by applying different static rules
   * Tutorial: A player that tries to teach basic strategies
   * Adaptive Tree AI: A variant of the Tree AI which tries to guess your next moves
   * Control AI: An AI that tries to control the board by conquering key positions
   * Assembly AI: An AI which uses several small entities to determine the best play
   * Neural Network AI: An AI using a neural network
   * Monte Carlo AI: An AI based on the idea of the "Monte Carlo method"

In addition there is an option to select a random AI. In this case, the messages of the AIs are surpressed. Restarting the game from the game bord will result in the same AI so you have an other chance of beat it.

---

HOWTO implement an own AI/Player:
   1. Inhert Player
   2. Overwrite doTurn(Gameboard board), isHuman(), humanInput(int x, int y)
   3. Add new AI to Gamemaster::initialise(QString player1, QString player2, int bonus)
   4. Add new AI to UI
