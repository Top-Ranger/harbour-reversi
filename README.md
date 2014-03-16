A reversi (also known as Othello) implementation for SailfishOS

This implementation features an easy way to implement different players.
Currently avaiable are:
   * Human: A human player
   * Random AI: A simple AI selecting a random tile
   * Greedy AI: Tries to get as many points as possible

---

HOWTO implement an own AI/Player:
   1. Inhert Player
   2. Overwrite doTurn(), isHuman(), getBoard(Gameboard board), humanInput(int x, int y)
   3. Add new AI to Gamemaster::initialise(QString player1, QString player2, int bonus)
   4. Add new AI to UI
