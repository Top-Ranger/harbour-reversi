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

#include "controlaiplayer.h"
#include <QTime>
#include <QDebug>

ControlAIPlayer::ControlAIPlayer(QObject *parent) :
    Player(parent),
    _availableFunctions(),
    _currentFunctions(),
    _priority()
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    // Fill _availableFunctions
    _availableFunctions[0] = &dummy;
    _availableFunctions[1] = &dummy2;

    // Fill _currentFunctions
    _currentFunctions[0] = _availableFunctions[qrand() % _sizeAvailableFunctions];
    _currentFunctions[1] = _availableFunctions[qrand() % _sizeAvailableFunctions];
    _currentFunctions[2] = _availableFunctions[qrand() % _sizeAvailableFunctions];
}


void ControlAIPlayer::doTurn()
{
    // Change function
    _currentFunctions[qrand() % _sizeCurrentFunctions] = _availableFunctions[qrand() % _sizeAvailableFunctions];

    // Initialising _priority
    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            if(((x = 0) || (x = 7)) && ((y = 0) || (y = 7)))
            {
                _priority[x][y] = 10;
            }
            else if(((x <= 1) || (x >= 6)) && ((y <= 1) || (y >= 6)))
            {
                _priority[x][y] = -10;
            }
            else
            {
                _priority[x][y] = 0;
            }
        }
    }

    emit wantBoard();
}

bool ControlAIPlayer::isHuman()
{
    return false;
}

void ControlAIPlayer::getBoard(Gameboard board, int player)
{
    int max = std::numeric_limits<int>::min();
    int x = qrand()%8;
    int y = qrand()%8;
    int xstart = x;
    int ystart = y;
    int xmax = -1;
    int ymax = -1;
    Gameboard testboard;

    for(int i = 0; i < _sizeCurrentFunctions; ++i)
    {
        _currentFunctions[i](board,player);
    }

    do
    {
        do
        {
            testboard = board;

            if(testboard.play(x,y,player))
            {
                int temp = 0;
                for(int tempx = 0; tempx < 8; ++tempx)
                {
                    for(int tempy = 0; tempy < 8; ++tempy)
                    {
                        if(testboard.owner(tempx,tempy) == player)
                        {
                            temp += _priority[tempx][tempy];
                        }
                    }
                }
                if(temp > max)
                {
                    max = temp;
                    xmax = x;
                    ymax = y;
                }
            }

            y = (y+1)%8;
        }while(y != ystart);

        x = (x+1)%8;
    }while(x != xstart);

    emit turn(xmax, ymax);
}

void ControlAIPlayer::humanInput(int x, int y)
{
}

void ControlAIPlayer::dummy(Gameboard board, int player)
{
    qWarning() << "Function" << 1;
}

void ControlAIPlayer::dummy2(Gameboard board, int player)
{
    qWarning() << "Function" << 2;
}
