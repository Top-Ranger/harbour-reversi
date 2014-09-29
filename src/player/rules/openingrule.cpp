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

#include "openingrule.h"
#include "rulehelper.h"
#include <limits>
#include <QTime>

using RuleHelper::isFrontierDisc;
using RuleHelper::canGetZeroDiscs;
using RuleHelper::opponent;

const int OpeningRule::_borderDiscs;
const int OpeningRule::_valueCenter;
const int OpeningRule::_factorDistribution;
const int OpeningRule::_valueFrontierDisc;

OpeningRule::OpeningRule(QObject *parent) :
    Rule(parent)
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
}


bool OpeningRule::applicable(Gameboard board, int player)
{
    return (board.points(player) + board.points(opponent(player))) <= _borderDiscs;
}

void OpeningRule::doTurn(Gameboard board, int player)
{
    int max = std::numeric_limits<int>::min();
    int x = qrand()%8;
    int y = qrand()%8;
    int xstart = x;
    int ystart = y;
    int xmax = -1;
    int ymax = -1;
    int xbad = -1;
    int ybad = -1;
    int badmax = std::numeric_limits<int>::min();
    Gameboard testboard;

    do
    {
        do
        {
            testboard = board;

            if(testboard.play(x,y,player))
            {
                int temp = calculateScore(testboard, player);
                if(canGetZeroDiscs(testboard,player))
                {
                    if(temp > badmax)
                    {
                        badmax = temp;
                        xbad = x;
                        ybad = y;
                    }
                }
                else
                {
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

    if(xmax == -1 || ymax == -1)
    {
        emit turn(xbad, ybad);
    }
    else
    {
        emit turn(xmax, ymax);
    }
}

QString OpeningRule::name()
{
    return tr("Opening Rule");
}

int OpeningRule::calculateScore(Gameboard board, int player)
{
    int score = (((float) board.points(opponent(player)))/board.points(player)) * _factorDistribution;

    for(int x = 3; x <=4; ++x)
    {
        for(int y = 3; y <=4; ++y)
        {
            if(board.owner(x,y) == player)
            {
                score += _valueCenter;
            }
            else if (board.owner(x,y) == opponent(player))
            {
                score -= _valueCenter;
            }
        }
    }

    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            if(isFrontierDisc(board,x,y))
            {
                if(board.owner(x,y) == player)
                {
                    score -= _valueFrontierDisc;
                }
                else if (board.owner(x,y) == opponent(player))
                {
                    score += _valueCenter;
                }
            }
        }
    }

    return score;
}
