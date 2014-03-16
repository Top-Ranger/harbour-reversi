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

#include "gamemaster.h"
#include "humanplayer.h"
#include "randomaiplayer.h"
#include "greedyaiplayer.h"
#include <QDebug>

Gamemaster::Gamemaster(QObject *parent) :
    QObject(parent),
    _bonus(0),
    _turn(1),
    _initialised(false)
{
    _player[0] = NULL;
    _player[1] = NULL;
    _board = NULL;
}

Gamemaster::~Gamemaster()
{
    cleanup();
}

bool Gamemaster::initialise(QString player1, QString player2, int bonus)
{
    cleanup();
    //Player 1
    if(player1 == "Human")
    {
        _player[0] = new HumanPlayer(this);
    }
    else if(player1 == "Random AI")
    {
        _player[0] = new RandomAIPlayer(this);
    }
    else if(player1 == "Greedy AI")
    {
        _player[0] = new GreedyAIPlayer(this);
    }
    else
    {
        return false;
    }
    QObject::connect(_player[0], SIGNAL(awaitsHuman()), this, SLOT(awaitsHuman()));
    QObject::connect(_player[0], SIGNAL(sendMessage(QString)), this, SLOT(message(QString)));
    QObject::connect(_player[0], SIGNAL(turn(int,int)), this, SLOT(turn(int,int)));
    QObject::connect(_player[0], SIGNAL(wantBoard()), this, SLOT(wantBoard()));
    QObject::connect(this,SIGNAL(humanInput(int,int)),_player[0],SLOT(humanInput(int,int)));

    //Player 2
    if(player2 == "Human")
    {
        _player[1] = new HumanPlayer(this);
    }
    else if(player2 == "Random AI")
    {
        _player[1] = new RandomAIPlayer(this);
    }
    else if(player2 == "Greedy AI")
    {
        _player[1] = new GreedyAIPlayer(this);
    }
    else
    {
        cleanup();
        return false;
    }
    QObject::connect(_player[1], SIGNAL(awaitsHuman()), this, SLOT(awaitsHuman()));
    QObject::connect(_player[1], SIGNAL(sendMessage(QString)), this, SLOT(message(QString)));
    QObject::connect(_player[1], SIGNAL(turn(int,int)), this, SLOT(turn(int,int)));
    QObject::connect(_player[1], SIGNAL(wantBoard()), this, SLOT(wantBoard()));
    QObject::connect(this,SIGNAL(humanInput(int,int)),_player[1],SLOT(humanInput(int,int)));

    _bonus = bonus;

    _board = new Gameboard(this);
    QObject::connect(_board, SIGNAL(boardChanged()),this,SLOT(getBoardChanged()));

    _initialised = true;
    return true;
}

void Gamemaster::getInput(int x, int y)
{
    if(!_initialised)
    {
        qCritical() << "Critical ERROR: Using Gamemaster without initialising it";
        return;
    }

    qDebug() << QString("Gamemaster is getting input %1 + %2").arg(x).arg(y);
    _player[_turn-1]->humanInput(x,y);
    emit humanInput(x, y);
}

void Gamemaster::cleanup()
{
    _initialised = false;
    if(_player[0] != NULL)
    {
        delete _player[0];
        _player[0] = NULL;
    }
    if(_player[1] != NULL)
    {
        delete _player[1];
        _player[1] = NULL;
    }
    if(_board != NULL)
    {
        QObject::disconnect(_board,SIGNAL(boardChanged()),this,SLOT(getBoardChanged()));
        delete _board;
        _board = NULL;
    }
}

void Gamemaster::awaitsHuman()
{
    qDebug() << "Gamemaster emits getHumanInput(_turn);";
    emit getHumanInput(_turn);
}

void Gamemaster::turn(int x, int y)
{
    if(!_initialised)
    {
        qCritical() << "Critical ERROR: Using Gamemaster without initialising it";
        return;
    }
    qDebug() << QString("Gamemaster is getting turn %1 + %2 from %3").arg(x).arg(y).arg(_turn);
    if(_board->play(x, y, _turn))
    {
        qDebug() << "Gamemaster: Turn possible!";
        _player[_turn-1]->isActive(false);
        _turn = _turn==1?2:1;
        if(!_board->isTurnPossible(_turn))
        {
            emit result(_board->points(1), _board->points(2)+_bonus);
            return;
        }
        else
        {
            _player[_turn-1]->isActive(true);
        }
    }
    else
    {
        qDebug() << "Gamemaster: Turn not possible!";
    }
    _player[_turn-1]->doTurn();
}

void Gamemaster::startGame()
{
    if(!_initialised)
    {
        qCritical() << "Critical ERROR: Using Gamemaster without initialising it";
        return;
    }

    if(_board->isTurnPossible(1))
    {
        _turn = 1;
        qDebug() <<  "Gamemaster starts the game!";
        _player[0]->isActive(true);
        _player[1]->isActive(false);
        _player[0]->doTurn();
    }
    else
    {
        emit result(_board->points(1), _board->points(2)+_bonus);
    }
}

int Gamemaster::getOwner(int x, int y)
{
    if(!_initialised)
    {
        qCritical() << "Critical ERROR: Using Gamemaster without initialising it";
        return 0;
    }
    return _board->owner(x, y);
}

int Gamemaster::pointsPlayer1()
{
    if(!_initialised)
    {
        qCritical() << "Critical ERROR: Using Gamemaster without initialising it";
        return 0;
    }

    return _board->points(1);
}

int Gamemaster::pointsPlayer2()
{
    if(!_initialised)
    {
        qCritical() << "Critical ERROR: Using Gamemaster without initialising it";
        return 0;
    }

    return _board->points(2)+_bonus;
}

void Gamemaster::wantBoard()
{
    if(!_initialised)
    {
        qCritical() << "Critical ERROR: Using Gamemaster without initialising it";
        return;
    }

    _player[_turn-1]->getBoard(Gameboard(*_board), _turn);
}

void Gamemaster::message(QString message)
{
    emit sendMessage(message);
}

void Gamemaster::getBoardChanged()
{
    qDebug() << "Gamemaster emits boardChanged();";
    emit boardChanged();
}
