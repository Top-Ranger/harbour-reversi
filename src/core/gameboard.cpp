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

#include "gameboard.h"
#include <QDebug>

Gameboard::Gameboard(QObject *parent) :
    QObject(parent)
{
    for(int i=0; i<8; ++i)
    {
        for(int j=0; j<8; ++j)
        {
            _board[i][j] = 0;
        }
    }

    _board[3][3]= 1;
    _board[3][4]= 2;
    _board[4][3]= 2;
    _board[4][4]= 1;
}

Gameboard::Gameboard(const Gameboard &other, QObject *parent) :
    QObject(parent)
{
    for(int i=0; i<8; ++i)
    {
        for(int j=0; j<8; ++j)
        {
            _board[i][j] = other._board[i][j];
        }
    }
}

Gameboard& Gameboard::operator=(const Gameboard& other)
{
    for(int i=0; i<8; ++i)
    {
        for(int j=0; j<8; ++j)
        {
            _board[i][j] = other._board[i][j];
        }
    }
    return *this;
}

bool Gameboard::play(int x, int y, int player, bool test)
{
    if(!checkCoordinates(x,y))
    {
        qCritical() << "FATAL ERROR in " __FILE__ << " " << __LINE__ << ": x or y value out of range";
        return false;
    }

    if(_board[x][y] != 0)
    {
        return false;
    }

    bool isPlayable = false;
    _board[x][y] = player;
    for(int deltax=-1; deltax<=1; ++deltax)
    {
        for(int deltay=-1; deltay<=1; ++deltay)
        {
            if(deltax != 0 || deltay != 0)
            {
                if(testLine(x, y, deltax, deltay, player, test))
                {
                    isPlayable = true;
                }
            }
        }
    }
    if(test || !isPlayable)
    {
        _board[x][y] = 0;
    }
    else
    {
        emit boardChanged();
    }

    return isPlayable;
}

bool Gameboard::isTurnPossible(int player)
{
    for(int i=0; i<8; ++i)
    {
        for(int j=0; j<8; ++j)
        {
            if(play(i, j, player, true))
            {
                return true;
            }
        }
    }
    return false;
}

int Gameboard::points(int player)
{
    int points = 0;
    for(int i=0; i<8; ++i)
    {
        for(int j=0; j<8; ++j)
        {
            if(_board[i][j] == player)
            {
                ++points;
            }
        }
    }
    return points;
}

int Gameboard::owner(int x, int y)
{
    if(!checkCoordinates(x,y))
    {
        qCritical() << "FATAL ERROR in " __FILE__ << __LINE__ << ": x or y value out of range";
        return -1;
    }

    return _board[x][y];
}

bool Gameboard::testLine(int x, int y, int deltax, int deltay, int player, bool test)
{
    if(!checkCoordinates(x,y))
    {
        qCritical() << "FATAL ERROR in " __FILE__ << __LINE__ << ": x or y value out of range";
        return false;
    }

    x += deltax;
    y += deltay;
    if(!checkCoordinates(x,y) || _board[x][y] != opponent(player))
    {
        return false;
    }
    while(true)
    {
        x += deltax;
        y += deltay;
        if(!checkCoordinates(x,y) || _board[x][y] == 0)
        {
            return false;
        }
        else if(_board[x][y] == player)
        {
            if(!test)
            {
                do
                {
                    _board[x][y] = player;
                    x -= deltax;
                    y -= deltay;
                } while(_board[x][y] != player);
            }
            return true;
        }
    }

}

bool inline Gameboard::checkCoordinates(int x, int y)
{
    return x>=0 && x<8 && y>=0 && y<8;
}

int inline Gameboard::opponent(int player)
{
    return player==1?2:1;
}
