/*
  Copyright (C) 2014,2016 Marcus Soll
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

#include "greedyaiplayer.h"

#include "../core/randomhelper.h"

GreedyAIPlayer::GreedyAIPlayer(QObject *parent) :
    Player(parent)
{
    RandomHelper::initialise();
}

bool GreedyAIPlayer::isHuman()
{
    return false;
}

void GreedyAIPlayer::doTurn(Gameboard board, int player)
{
    int max = -1;
    int old = board.points(player);
    int x = RandomHelper::random_place();
    int y = RandomHelper::random_place();
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
                if(testboard.points(player) > max)
                {
                    max = testboard.points(player);
                    xmax = x;
                    ymax = y;
                }
            }

            y = (y+1)%8;
        }while(y != ystart);

        x = (x+1)%8;
    }while(x != xstart);

    if((max-old) >= 6)
    {
        emit sendMessage(QString(tr("HA! %1 points!")).arg(max-old));
    }
    else
    {
        emit sendMessage(QString(tr("OH NO! Only %1 points")).arg(max-old));
    }
    emit turn(xmax, ymax);
}

void GreedyAIPlayer::humanInput(int x, int y)
{
    // Do nothing on human input
    Q_UNUSED(x)
    Q_UNUSED(y)
}
