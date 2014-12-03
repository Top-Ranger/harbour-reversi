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

#include "randomaiplayer.h"
#include <QTime>

RandomAIPlayer::RandomAIPlayer(QObject *parent) :
    Player(parent)
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
}

bool RandomAIPlayer::isHuman()
{
    return false;
}

void RandomAIPlayer::doTurn(Gameboard board, int player)
{
    int random = qrand()%10;

    if(random == 0)
    {
        emit sendMessage(tr("What am I doing?"));
    }
    else
    {
        QString s = "";
        for(int i = 0; i < random; --random)
        {
            s = QString("%1%2").arg(s).arg("?");
        }
        emit sendMessage(s);
    }

    int x = qrand()%8;
    int y = qrand()%8;
    int xstart = x;
    int ystart = y;

    do
    {
        do
        {
            if(board.play(x,y,player,true))
            {
                emit turn(x,y);
                return;
            }
            y = (y+1)%8;
        }while(y != ystart);

        x = (x+1)%8;
    }while(x != xstart);

    emit turn(qrand()%8, qrand()%8);
}

void RandomAIPlayer::humanInput(int x, int y)
{
}
