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

#include "treeaiplayer.h"
#include <QTime>
#include <limits>

TreeAIPlayer::TreeAIPlayer(QObject *parent) :
    Player(parent)
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
}

bool TreeAIPlayer::isHuman()
{
    return false;
}

void TreeAIPlayer::doTurn(Gameboard board, int player)
{
    float max = -1048576;
    int x = qrand()%8;
    int y = qrand()%8;
    int xstart = x;
    int ystart = y;
    int xmax = -1;
    int ymax = -1;
    Gameboard testboard;
    int composure = qrand() % _composure;

    do
    {
        do
        {
            testboard = board;

            if(testboard.play(x,y,player))
            {
                if(composure == 0)
                {
                    if(testboard.points(player) > max)
                    {
                        max = testboard.points(player);
                        xmax = x;
                        ymax = y;
                    }
                }
                else
                {
                    float temp = buildTree(testboard, opponent(player), player, board.points(player), board.points(opponent(player)), _depth);
                    if(temp > max)
                    {
                        max = temp;
                        xmax = x;
                        ymax = y;
                    }
                }
            }

            y = (y+1)%8;
        }while(y != ystart);

        x = (x+1)%8;
    }while(x != xstart);

    if(composure == 0)
    {
        emit sendMessage(QString(tr("I want %1:%2! NOW!")).arg(xmax).arg(ymax));
    }
    else
    {
        emit sendMessage(QString(tr("I'll do %1:%2, that should be the best turn with factor %3")).arg(xmax).arg(ymax).arg(max));
    }
    emit turn(xmax,ymax);
}

void TreeAIPlayer::humanInput(int x, int y)
{
}

float TreeAIPlayer::buildTree(Gameboard board, int player, int I, int old, int opponentOld, int depth)
{
    --depth;
    int n = 0;
    float advantage = 0;

    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            if(board.play(x,y,player,true))
            {
                if(depth <= 0)
                {
                    if(player == I)
                    {
                        advantage += _valueMove;
                    }
                    else
                    {
                        advantage -= _valueMove;
                    }
                    Gameboard newBoard = board;
                    newBoard.play(x,y,player,false);
                    advantage += calculateScore(newBoard, I,newBoard.points(I)-old, newBoard.points(opponent(I))-opponentOld);
                    ++n;
                }
                else
                {
                    if(player == I)
                    {
                        advantage += _valueMove;
                    }
                    else
                    {
                        advantage -= _valueMove;
                    }
                    Gameboard newBoard = board;
                    newBoard.play(x,y,player,false);
                    advantage += buildTree(newBoard,opponent(player),I,old,opponentOld,depth);
                    ++n;
                }
            }
        }
    }
    if(n == 0)
    {
        if(board.points(I) > board.points(opponent(I)))
        {
            return _valueFullBoard;
        }
        else
        {
            return -_valueFullBoard;
        }
    }
    else
    {
        return advantage / n;
    }
}

int inline TreeAIPlayer::opponent(int player)
{
    return player==1?2:1;
}

float TreeAIPlayer::calculateScore(Gameboard board, int I, int change, int opponentChange)
{
    float score = 0;

    score += change * _factorChange;
    score -= opponentChange * _factorOpponentChange;

    if(board.points(I) > board.points(opponent(I)))
    {
        score += _factorRatio * (board.points(I)/(board.points(opponent(I))+1));
    }
    else if(board.points(I) < board.points(opponent(I)))
    {
        score -= _factorRatio * (board.points(I)/(board.points(opponent(I))+1));
    }

    if(board.owner(0,0) == I)
    {
        score += _valueCorner;
    }
    else if(board.owner(0,0) == opponent(I))
    {
        score -= _valueCorner;
    }
    else if(board.owner(0,0) == 0)
    {
        if(board.owner(1,1) == I)
        {
            score -= _valueCorner;
        }
        else if(board.owner(1,1) == opponent(I))
        {
            score += _valueCorner;
        }
    }

    if(board.owner(7,0) == I)
    {
        score += _valueCorner;
    }
    else if(board.owner(7,0) == opponent(I))
    {
        score -= _valueCorner;
    }
    else if(board.owner(7,0) == 0)
    {
        if(board.owner(6,1) == I)
        {
            score -= _valueCorner;
        }
        else if(board.owner(6,1) == opponent(I))
        {
            score += _valueCorner;
        }
    }

    if(board.owner(0,7) == I)
    {
        score += _valueCorner;
    }
    else if(board.owner(0,7) == opponent(I))
    {
        score -= _valueCorner;
    }
    else if(board.owner(0,7) == 0)
    {
        if(board.owner(1,6) == I)
        {
            score -= _valueCorner;
        }
        else if(board.owner(1,6) == opponent(I))
        {
            score += _valueCorner;
        }
    }

    if(board.owner(7,7) == I)
    {
        score += _valueCorner;
    }
    else if(board.owner(7,7) == opponent(I))
    {
        score -= _valueCorner;
    }
    else if(board.owner(7,7) == 0)
    {
        if(board.owner(6,6) == I)
        {
            score -= _valueCorner;
        }
        else if(board.owner(6,6) == opponent(I))
        {
            score += _valueCorner;
        }
    }

    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; y++)
        {
            int valueStone;

            if(x == 7 || x == 0 || y == 7 || y == 0)
            {
                valueStone = 7;
            }
            else if(x == 6 || x == 1 || y == 6 || y == 1)
            {
                valueStone = 3;
            }
            else if((x == 3 || x == 4) && (y == 3 && y == 4))
            {
                valueStone = 5;
            }
            else
            {
                valueStone = 1;
            }

            if(board.owner(x,y) == I)
            {
                score += _factorStone * valueStone;
            }
            else if(board.owner(x,y) == opponent(I))
            {
                score -= _factorStone * valueStone;
            }
        }
    }
    if(!board.isTurnPossible(opponent(I)))
    {
        if(board.points(I) > board.points(opponent(I)))
        {
            score += _valueFullBoard;
        }
        else
        {
            score += -_valueFullBoard;
        }
    }
    return score;
}
