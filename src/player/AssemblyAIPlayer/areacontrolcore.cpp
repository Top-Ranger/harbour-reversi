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

#include "areacontrolcore.h"
#include "assemblyaihelper.h"

namespace {
int countUncontrolledAreaLayer(Gameboard board, int player, int x, int y)
{
    int result = 0;
    bool test = true;

    while(test && result < 8)
    {
        int tempx = x-result;
        int tempy = y-result;

        while(test && (tempx <= (x+result)))
        {
            if((tempx >= 0) && (tempx <= 7) && (tempy >= 0) && (tempy <= 7))
            {
                if(board.owner(tempx,tempy) == player)
                {
                    test = false;
                }
            }
            ++tempx;
        }
        --tempx;

        while(test && (tempy <= (x+result)))
        {
            if((tempx >= 0) && (tempx <= 7) && (tempy >= 0) && (tempy <= 7))
            {
                if(board.owner(tempx,tempy) == player)
                {
                    test = false;
                }
            }
            ++tempy;
        }
        --tempy;

        while(test && (tempx >= (x-result)))
        {
            if((tempx >= 0) && (tempx <= 7) && (tempy >= 0) && (tempy <= 7))
            {
                if(board.owner(tempx,tempy) == player)
                {
                    test = false;
                }
            }
            --tempx;
        }
        ++tempx;

        while(test && (tempy >= (x-result)))
        {
            if((tempx >= 0) && (tempx <= 7) && (tempy >= 0) && (tempy <= 7))
            {
                if(board.owner(tempx,tempy) == player)
                {
                    test = false;
                }
            }
            --tempy;
        }

        if(test)
        {
            ++result;
        }
    }
    return result;
}
}

AreaControlCore::AreaControlCore()
{
}

bool AreaControlCore::retirement(Gameboard board, int player)
{
    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            if(countUncontrolledAreaLayer(board,player,x,y) >=3)
            {
                return false;
            }
        }
    }
    return true;
}

int AreaControlCore::mistrust(float const* const* const vote, Gameboard board, int player)
{
    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            if(vote[x][y] > 0 && countUncontrolledAreaLayer(board,player,x,y) >=1)
            {
                return Core::_noMistrust;
            }
        }
    }
    return Core::_mistrustSmall;
}

void AreaControlCore::propose(float ** const vote, Gameboard board, int player)
{
    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            if(board.play(x,y,player,true))
            {
                Gameboard testboard = board;
                testboard.play(x,y,player,false);
                vote[x][y] = 1/(9-countUncontrolledAreaLayer(board,player,x,y));
            }
        }
    }
}

void AreaControlCore::correct(float ** const vote, Gameboard board, int player)
{
    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            if(board.play(x,y,player,true))
            {
                Gameboard testboard = board;
                testboard.play(x,y,player,false);
                switch(countUncontrolledAreaLayer(board,player,x,y))
                {
                case 0:
                    vote[x][y] /= Core::_factorLarge;
                    break;
                case 1:
                    break;
                case 2:
                    vote[x][y] *=Core::_factorSmall;
                    break;
                default:
                    vote[x][y] *=Core::_factorLarge;
                    break;
                }
            }
        }
    }
    AssemblyAI::ensureNoIllegalMove(vote,board,player);
}

QString AreaControlCore::name() const
{
    return "Area Control Core";
}
