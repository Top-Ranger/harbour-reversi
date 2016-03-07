/*
  Copyright (C) 2016 Marcus Soll
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

#include "montecarloplayer.h"

#include <QTime>

static int opponent(int player){return player==1?2:1;}

static bool singleRunMonteCarlo(Gameboard board, int player, int current_player)
{
    while(board.isTurnPossible(current_player))
    {
        // Opponent will play corners if possible
        if(player != current_player && board.play(0,0,current_player))
        {
            continue;
        }
        else if(player != current_player && board.play(0,7,current_player))
        {
            continue;
        }
        else if(player != current_player && board.play(7,0,current_player))
        {
            continue;
        }
        else if(player != current_player && board.play(7,7,current_player))
        {
            continue;
        }
        else
        {
            // Random play
            bool possible = false;
            int x = qrand()%8;
            int y = qrand()%8;
            int xstart = x;
            int ystart = y;

            do
            {
                do
                {
                    if(board.play(x,y,current_player))
                    {
                        possible = true;
                    }

                    y = (y+1)%8;
                } while(y != ystart && !possible);

                x = (x+1)%8;
            } while(x != xstart && !possible);
        }
        current_player = opponent(current_player);
    }

    return board.points(player) > board.points(opponent(player));
}

static int runMonteCarlo(Gameboard board, int player, int current_player)
{
    int points = 0;
    for(int i = 0; i < MonteCatloPlayer::NUM_RUNS; ++i)
    {
        if(singleRunMonteCarlo(board, player, current_player))
        {
            ++points;
        }
    }
    return points;
}

MonteCatloPlayer::MonteCatloPlayer(QObject *parent) :
    Player(parent)
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
}

bool MonteCatloPlayer::isHuman()
{
    return false;
}

void MonteCatloPlayer::doTurn(Gameboard board, int player)
{
    int max = -1;
    int max_bad = -1;
    int x = qrand()%8;
    int y = qrand()%8;
    int xstart = x;
    int ystart = y;
    int xmax = -1;
    int xmax_bad = -1;
    int ymax = -1;
    int ymax_bad = -1;
    Gameboard testboard;

    if(board.play(0,0,player,true))
    {
        emit sendMessage(QString(tr("Monte Carlo method returns %1")).arg(NUM_RUNS));
        emit turn(0,0);
        return;
    }

    if(board.play(0,7,player,true))
    {
        emit sendMessage(QString(tr("Monte Carlo method returns %1")).arg(NUM_RUNS));
        emit turn(0,7);
        return;
    }

    if(board.play(7,0,player,true))
    {
        emit sendMessage(QString(tr("Monte Carlo method returns %1")).arg(NUM_RUNS));
        emit turn(7,0);
        return;
    }

    if(board.play(7,7,player,true))
    {
        emit sendMessage(QString(tr("Monte Carlo method returns %1")).arg(NUM_RUNS));
        emit turn(7,7);
        return;
    }

    do
    {
        do
        {
            testboard = board;

            if(testboard.play(x,y,player))
            {
                int points = runMonteCarlo(testboard, player, opponent(player));
                if(points > max)
                {
                    max_bad = points;
                    xmax_bad = x;
                    ymax_bad = y;
                    if(!(board.play(0,0,opponent(player),true) || board.play(0,7,opponent(player),true) || board.play(7,0,opponent(player),true) || board.play(7,7,opponent(player),true)))
                    {
                        max = points;
                        xmax = x;
                        ymax = y;
                    }
                }
            }

            y = (y+1)%8;
        }while(y != ystart);

        x = (x+1)%8;
    }while(x != xstart);

    if(max != -1)
    {
        emit sendMessage(QString(tr("Monte Carlo method returns %1")).arg(max));
        emit turn(xmax, ymax);
    }
    else
    {
        emit sendMessage(QString(tr("Monte Carlo method returns %1")).arg(max_bad));
        emit turn(xmax_bad, ymax_bad);
    }
}

void MonteCatloPlayer::humanInput(int x, int y)
{
    // Do nothing on human input
    Q_UNUSED(x)
    Q_UNUSED(y)
}