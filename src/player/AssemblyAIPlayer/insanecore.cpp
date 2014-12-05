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

#include "insanecore.h"
#include <QTime>

int InsaneCore::_counter = 0;

InsaneCore::InsaneCore() :
    Core(),
    _id(++_counter)
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
}

bool InsaneCore::retirement(Gameboard board, int player)
{
    return (qrand()%2 == 0);
}

int InsaneCore::mistrust(float const* const* const vote, Gameboard board, int player)
{
    return (qrand()%60 == 0)?Core::_factorSmall:Core::_noMistrust;
}

void InsaneCore::propose(float ** const vote, Gameboard board, int player)
{
    int count = 0;
    do{
        for(int i = 0; i < 10 && count < 3; ++i)
        {
            int x = qrand()%8;
            int y = qrand()%8;
            if(board.play(x,y,player,true))
            {
                vote[x][y] = 1;
                ++count;
            }
        }
    }while(count == 0);
}

void InsaneCore::correct(float ** const vote, Gameboard board, int player)
{
    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            if(vote[x][y] != 0)
            {
                switch(qrand()%8)
                {
                case 0:
                    vote[x][y] *= Core::_factorSmall;
                    break;
                case 1:
                    vote[x][y] *= Core::_factorLarge;
                    break;
                case 2:
                    vote[x][y] /= Core::_factorSmall;
                    break;
                case 3:
                    vote[x][y] /= Core::_factorLarge;
                    break;
                default:
                    break;
                }
            }
        }
    }
}

QString InsaneCore::name() const
{
    return QString("InsaneCore Nr. %1").arg(_id);
}
