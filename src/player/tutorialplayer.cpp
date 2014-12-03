/*
  Copyright (C) 2014 Marcus Soll
  Copyright (C) 2014 Johannes Preuß
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

#include "tutorialplayer.h"

TutorialPlayer::TutorialPlayer(QObject *parent) :
    Player(parent),
    _board(),
    _player(0),
    _x(-1),
    _y(-1),
    _currentRound(0)
{
}

bool TutorialPlayer::isHuman()
{
    return true;
}

void TutorialPlayer::doTurn(Gameboard board, int player)
{
    ++_currentRound;
    if(_currentRound == 1)
    {
        emit sendMessage(tr("Welcome to Reversi!\nYou can place a disc on an empty field, however you have to conquer at least one opponent disc.\nYou can do this by trapping it between the currently played disc and an already existing one."));
    }
    else if(_currentRound == 2)
    {
        emit sendMessage(tr("The goal of the game is to capture more discs than your opponent at the end of the game.\nThe game ends when neither player can place a disc on the gameboard.\nHaving a lot of discs on the board (before the end) does not automatically mean that you win."));
    }
    else if(_currentRound == 3)
    {
        emit sendMessage(tr("In the following part of the tutorial you will get some hints on how to play more efficiently.\nReversi is a game of many strategies so it is hard to tell whether you made the right move or not.\nType any position to get an evaluation of that move"));
    }
    else
    {
        emit sendMessage(tr("Type any position to get an evaluation"));
    }

    _board = board;
    _player = player;

    emit awaitsHuman();
}

void TutorialPlayer::humanInput(int x, int y)
{
    if(_active)
    {
        if(x == _x && y == _y)
        {
            emit turn(x,y);
            _x = -1;
            _y = -1;
            return;
        }
        Gameboard testboard = _board;

        if(testboard.play(x,y,_player))
        {
            if(_currentRound < 3)
            {
                emit turn(x,y);
                return;
            }

            QString s;
            if((x == 0 || x == 7) && (y == 0 || y == 7))
            {
                s = QString(tr("This is a corner.\nDiscs played in a corner are normally considered valuable because your opponent can't conquer it.\n"));
            }
            else if(_board.play(0,0,_player,true) || _board.play(0,7,_player,true) || _board.play(7,0,_player,true) || _board.play(7,7,_player,true))
            {
                s = QString(tr("You can play a disc into a corner.\nBecause these discs are considered valuable you might consider playing your disc there.\n"));
            }
            else if((_board.points(_player) + _board.points(opponentPlayer(_player))) <= _borderEarlyGame)
            {
                s = QString(tr("This is the early game.\nTry to assume control of the center of the board.\nTry to avoid the outer fields for now (few frontier discs).\n"));
                int own = 0;
                int opponent = 0;
                for(int newX = 3; newX <=4; ++newX)
                {
                    for(int newY = 3; newY <=4; ++newY)
                    {
                        if(testboard.owner(newX,newY) == _player)
                        {
                            ++own;
                        }
                        else if(testboard.owner(newX,newY) == opponentPlayer(_player))
                        {
                            ++opponent;
                        }
                    }
                }
                s = QString(tr("%1   Own discs in centre: %2\n   Opponent discs in centre: %3\n")).arg(s).arg(own).arg(opponent);

                own = 0;
                opponent = 0;
                for(int newX = 0; newX < 8; ++newX)
                {
                    for(int newY = 0; newY < 8; ++newY)
                    {
                        if(testboard.owner(newX,newY) == _player  && isFrontierDisc(testboard,newX,newY))
                        {
                            ++own;
                        }
                        else if(testboard.owner(newX,newY) == opponentPlayer(_player)  && isFrontierDisc(testboard,newX,newY))
                        {
                            ++opponent;
                        }
                    }
                }
                s = QString(tr("%1   Own frontier discs: %2\n   Opponent frontier discs: %3\n")).arg(s).arg(own).arg(opponent);
            }
            else if((_board.points(_player) + _board.points(opponentPlayer(_player))) >= _borderEndgame)
            {
                s = QString(tr("This is the end-game.\nTry to use all your advantages to get as many discs as possible.\n"));
                int own = 0;
                int opponent = 0;
                for(int newX = 0; newX < 8; ++newX)
                {
                    for(int newY = 0; newY < 8; ++newY)
                    {
                        if(testboard.owner(newX,newY) == _player)
                        {
                            ++own;
                        }
                        else if(testboard.owner(newX,newY) == opponentPlayer(_player))
                        {
                            ++opponent;
                        }
                    }
                }
                s = QString(tr("%1   Own discs: %2\n   Opponent discs: %3\n")).arg(s).arg(own).arg(opponent);
            }
            else
            {
                s = QString(tr("This is the mid-game.\nTry to get as many possible moves while reducing the number of moves your opponent can do.\n"));
                int own = 0;
                int opponent = 0;
                for(int newX = 0; newX < 8; ++newX)
                {
                    for(int newY = 0; newY < 8; ++newY)
                    {
                        if(testboard.play(newX, newY,_player,true))
                        {
                            ++own;
                        }
                        if(testboard.play(newX, newY,opponentPlayer(_player),true))
                        {
                            ++opponent;
                        }
                    }
                }
                s = QString(tr("%1   Own moves: %2\n   Opponent moves: %3\n")).arg(s).arg(own).arg(opponent);
            }

            emit sendMessage(QString(tr("%1Type again to place a disc here.")).arg(s));
            _x = x;
            _y = y;
            emit awaitsHuman();
            return;
        }
        else
        {
            emit sendMessage(tr("You can't place a disc there!"));
            _x = -1;
            _y = -1;
            emit awaitsHuman();
            return;
        }
    }
}

int TutorialPlayer::opponentPlayer(int player)
{
    return player==1?2:1;
}

bool TutorialPlayer::isFrontierDisc(Gameboard board, int x, int y)
{
    if(board.owner(x,y) == 0)
    {
        return false;
    }

    for(int deltax=-1; deltax<=1; ++deltax)
    {
        for(int deltay=-1; deltay<=1; ++deltay)
        {
            if(deltax != 0 || deltay != 0)
            {
                if((x+deltax >= 0) && (y+deltay >= 0) && (x+deltax <= 7) && (y+deltay <= 7))
                {
                    if(board.owner(x+deltax,y+deltay) == 0)
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
