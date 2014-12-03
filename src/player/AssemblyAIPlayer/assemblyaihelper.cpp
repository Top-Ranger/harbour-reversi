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

#include "assemblyaihelper.h"
#include <QDebug>

namespace {
bool findOneFreePlace(float ** const vote, Gameboard board, int player, bool ignoreOpponentCanGetCorner=true)
{
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
                if(ignoreOpponentCanGetCorner)
                {
                    vote[x][y] = 1;
                    return true;
                }
                else
                {
                    if(board.owner(0,0) == 0 && (( x==0 && y==1 ) || ( x==1 && y==0 ) || ( x==1 && y==1 )))
                    {
                        y = (y+1)%8;
                        continue;
                    }
                    if(board.owner(0,7) == 0 && (( x==0 && y==6 ) || ( x==1 && y==7 ) || ( x==1 && y==6 )))
                    {
                        y = (y+1)%8;
                        continue;
                    }
                    if(board.owner(7,0) == 0 && (( x==7 && y==1 ) || ( x==6 && y==0 ) || ( x==6 && y==1 )))
                    {
                        y = (y+1)%8;
                        continue;
                    }
                    if(board.owner(7,7) == 0 && (( x==7 && y==6 ) || ( x==6 && y==7 ) || ( x==6 && y==6 )))
                    {
                        y = (y+1)%8;
                        continue;
                    }
                    Gameboard testboard = board;
                    board.play(x,y,player,false);
                    if(!(testboard.play(0,0,AssemblyAI::opponent(player),true) || testboard.play(0,7,AssemblyAI::opponent(player),true) || testboard.play(7,0,AssemblyAI::opponent(player),true) || testboard.play(7,7,AssemblyAI::opponent(player),true)))
                    {
                        vote[x][y] = 1;
                        return true;
                    }
                }
            }
            y = (y+1)%8;
        }while(y != ystart);

        x = (x+1)%8;
    }while(x != xstart);
    return false;
}
}

void AssemblyAI::ensureOnePossibleMove(float ** const vote, Gameboard board, int player)
{
    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            if(vote[x][y] > 0 && board.play(x,y,player,true))
            {
                return;
            }
        }
    }

    if(board.play(0,0,player,true))
    {
        vote[0][0] = 1;
        return;
    }
    else if(board.play(0,7,player,true))
    {
        vote[0][7] = 1;
        return;
    }
    else if(board.play(7,0,player,true))
    {
        vote[7][0] = 1;
        return;
    }
    else if(board.play(7,7,player,true))
    {
        vote[7][7] = 1;
        return;
    }

    if(findOneFreePlace(vote,board,player,false))
    {
        return;
    }
    if(findOneFreePlace(vote,board,player,true))
    {
        return;
    }
    qCritical() << "FATAL ERROR in " __FILE__ << " " << __LINE__ << ": No possible move!";
}

void AssemblyAI::ensureNoIllegalMove(float ** const vote, Gameboard board, int player)
{
    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            if(vote[x][y] > 0 && !board.play(x,y,player,true))
            {
                vote[x][y] = 0;
            }
        }
    }
    ensureOnePossibleMove(vote, board, player);
}
