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

#ifndef GAMEMASTER_H
#define GAMEMASTER_H

#include <QObject>
#include "../player/player.h"
#include "gameboard.h"

class Gamemaster : public QObject
{
    Q_OBJECT
public:
    explicit Gamemaster(QObject *parent = 0);
    ~Gamemaster();
    Q_INVOKABLE bool initialise(QString player1, QString player2, int bonus);
    Q_INVOKABLE void getInput(int x, int y);
    Q_INVOKABLE void cleanup();
    Q_INVOKABLE void startGame();
    Q_INVOKABLE int getOwner(int x, int y);
    Q_INVOKABLE int pointsPlayer1();
    Q_INVOKABLE int pointsPlayer2();

signals:
    void humanInput(int x, int y);
    void getHumanInput(int player);
    void changeActivePlayer(bool isHuman);
    void result(int player1, int player2);
    void sendMessage(QString message);
    void boardChanged();
    void lastDiscPlayed(int x, int y);

public slots:
    void awaitsHuman();
    void turn(int x, int y);
    void message(QString message);
    void getBoardChanged();

private:
    Player *_player[2];
    int _bonus;
    int _turn;
    Gameboard *_board;
    bool _initialised;

};

#endif // GAMEMASTER_H
