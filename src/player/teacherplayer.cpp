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

#include "teacherplayer.h"

TeacherPlayer::TeacherPlayer(QObject *parent) :
    Player(parent),
    _board(),
    _player(0),
    _x(-1),
    _y(-1)
{
}

void TeacherPlayer::doTurn()
{
    emit wantBoard();
    emit awaitsHuman();
    emit sendMessage("Type any position to get an evaluation");
}

bool TeacherPlayer::isHuman()
{
    return true;
}

void TeacherPlayer::getBoard(Gameboard board, int player)
{
    _board = board;
    _player = player;
}

void TeacherPlayer::humanInput(int x, int y)
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
            QString s;
            if((_board.points(_player) + _board.points(opponentPlayer(_player))) <= _borderEarlyGame)
            {
                s = QString("This is the early game\nTry to get the centre of the board\nTry to get as few discs at the border as possible\n");
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
                s = QString("%1Own discs in centre: %2\nOpponent discs in centre: %3\n").arg(s).arg(own).arg(opponent);

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
                s = QString("%1Own frontier discs: %2\nOpponent frontier discs: %3\n").arg(s).arg(own).arg(opponent);
            }
            else if((_board.points(_player) + _board.points(opponentPlayer(_player))) >= _borderEndgame)
            {
                s = QString("This is the Endgame\nTry to use all your adventages to get as many discs as possible\n");
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
                s = QString("%1Own discs: %2\nOpponent discs: %3\n").arg(s).arg(own).arg(opponent);
            }
            else
            {
                s = QString("This is the Midgame\nTry to get as many possible plays while reducing the number of plays your opponent can do\n");
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
                s = QString("%1Own plays: %2\nOpponent plays: %3\n").arg(s).arg(own).arg(opponent);
            }

            emit sendMessage(QString("%1Type again to play disc here").arg(s));
            _x = x;
            _y = y;
            emit awaitsHuman();
            return;
        }
        else
        {
            emit sendMessage("You can't play a disc there!");
            _x = -1;
            _y = -1;
            emit awaitsHuman();
            return;
        }
    }
}

int TeacherPlayer::opponentPlayer(int player)
{
    return player==1?2:1;
}

bool TeacherPlayer::isFrontierDisc(Gameboard board, int x, int y)
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
