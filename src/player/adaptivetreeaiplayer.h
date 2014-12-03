/*
  Copyright (C) 2014 Marcus Soll
  All rights reserved.

  You may use this file under the terms of BSD license as follows:

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Jolla Ltd nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef ADAPTIVETREEAIPLAYER_H
#define ADAPTIVETREEAIPLAYER_H

#include "player.h"

class AdaptiveTreeAIPlayer : public Player
{
    Q_OBJECT
public:
    explicit AdaptiveTreeAIPlayer(QObject *parent = 0);
    virtual bool isHuman();
    virtual void doTurn(Gameboard board, int player);

public slots:
    virtual void humanInput(int x, int y);

private:
    float buildTree(Gameboard board, int player, int I, int old, int opponentOld, int depth);
    int inline opponent(int player);
    float calculateScore(Gameboard board, int I, int change, int opponentChange);

    bool triesFewerFrontierDiscs(Gameboard board, int x, int y, int opponentValue);
    bool triesFewerMoves(Gameboard board, int x, int y, int opponentValue);
    bool triesMoreOwnMoves(Gameboard board, int x, int y, int opponentValue);
    bool triesGreedy(Gameboard board, int x, int y, int opponentValue);
    bool triesEdge(Gameboard board, int x, int y, int opponentValue);
    bool triesLonely(Gameboard board, int x, int y, int opponentValue);

    void adjustFactors(Gameboard board, int x, int y, int opponentValue);
    float calculateFactor(Gameboard board, int x, int y, int opponentValue);

    bool isFrontierDisc(Gameboard board, int x, int y);

    bool _firstBoard;
    float _fewerFrontierDiscs;
    float _fewerMoves;
    float _moreOwnMoves;
    float _greedy;
    float _edge;
    float _lonely;

    Gameboard _oldBoard;

    static const int _depth = 2; // 2
    static const int _composure = 10; // 10
    static const int _valueCorner = 100; // 100
    static const float _factorStone = 0.37; // 0.37
    static const int _valueFullBoard = 500; // 500
    static const float _factorRatio = 40; // 40
    static const float _factorChange = 15.5; // 15.5
    static const float _factorOpponentChange = 25.5; // 25.5
    static const float _valueMove = 4; // 4

    static const float _factorLearnung = 3; // 3
    static const float _upperBound = 8; // 8
    static const float _lowerBound = 0.125; // 0.125

    static const int _borderLonely = 2; // 2
};

#endif // ADAPTIVETREEAIPLAYER_H
