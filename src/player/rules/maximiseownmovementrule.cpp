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

#include "maximiseownmovementrule.h"
#include "rulehelper.h"
#include <QDebug>
#include <QTime>

using RuleHelper::canTakeCorner;
using RuleHelper::canGetZeroDiscs;
using RuleHelper::opponent;

const int MaximiseOwnMovementRule::_borderMoves;

MaximiseOwnMovementRule::MaximiseOwnMovementRule(QObject *parent) :
    Rule(parent),
    _x(-1),
    _y(-1),
    _asked(false)
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
}

bool MaximiseOwnMovementRule::applicable(Gameboard board, int player)
{
    int max = -1;
    int x = qrand()%8;
    int y = qrand()%8;
    int xstart = x;
    int ystart = y;
    int xmax = -1;
    int ymax = -1;
    Gameboard testboard;

    do
    {
        do
        {
            testboard = board;

            if(testboard.play(x,y,player))
            {
                int temp = 0;

                for(int newX = 0; newX < 8; ++newX)
                {
                    for(int newY = 0; newY < 8; ++newY)
                    {
                        if(testboard.play(newX, newY,player,true))
                        {
                            ++temp;
                        }
                    }
                }

                if(temp > max && (!canTakeCorner(testboard, opponent(player))) && (!canGetZeroDiscs(testboard,player)))
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

    if(max >= _borderMoves)
    {
        _x = xmax;
        _y = ymax;
        _asked = true;
    }
    else
    {
        _x = -1;
        _y = -1;
        _asked = false;
    }
    return _asked;
}

void MaximiseOwnMovementRule::doTurn(Gameboard board, int player)
{
    if(_asked)
    {
        _asked = false;
        emit turn(_x,_y);
        _x = -1;
        _y = -1;
    }
    else
    {
        qCritical() << "FATAL ERROR in " __FILE__ << " " << __LINE__ << ": MaximiseOwnMovementRule::applicable(Gameboard board, int player) was not called";
    }
}

QString MaximiseOwnMovementRule::name()
{
    return tr("Maximise Own Movement Rule");
}
