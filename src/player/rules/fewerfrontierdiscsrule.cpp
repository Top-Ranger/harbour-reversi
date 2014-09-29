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

#include "fewerfrontierdiscsrule.h"
#include "rulehelper.h"
#include <QTime>

using RuleHelper::isFrontierDisc;
using RuleHelper::canTakeCorner;
using RuleHelper::canGetZeroDiscs;
using RuleHelper::opponent;

FewerFrontierDiscsRule::FewerFrontierDiscsRule(QObject *parent) :
    Rule(parent)
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
}

bool FewerFrontierDiscsRule::applicable(Gameboard board, int player)
{
    return board.isTurnPossible(player);
}

void FewerFrontierDiscsRule::doTurn(Gameboard board, int player)
{
    int max = 100;
    int x = qrand()%8;
    int y = qrand()%8;
    int xstart = x;
    int ystart = y;
    int xmax = -1;
    int ymax = -1;
    int xmaxbad = -1;
    int ymaxbad = -1;
    int maxbad = 100;
    Gameboard testboard;

    do
    {
        do
        {
            testboard = board;
            int temp = 0;

            if(testboard.play(x,y,player))
            {
                for(int newX = 0; newX < 8; ++newX)
                {
                    for(int newY = 0; newY < 8; ++newY)
                    {
                        if(testboard.owner(newX,newY) == player && isFrontierDisc(testboard,newX,newY))
                        {
                            ++temp;
                        }
                    }
                }

                if(canTakeCorner(testboard,opponent(player)) || canGetZeroDiscs(testboard,player))
                {
                    if(temp < maxbad)
                    {
                        maxbad = temp;
                        xmaxbad = x;
                        ymaxbad = y;
                    }
                }
                else
                {
                    if(temp < max)
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

    if((xmax != -1) && (ymax != -1))
    {
        emit turn(xmax, ymax);
    }
    else
    {
        emit turn(xmaxbad, ymaxbad);
    }
}

QString FewerFrontierDiscsRule::name()
{
    return tr("Fewer Frontier Discs Rule");
}
