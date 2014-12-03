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

#include "movementcore.h"
#include "assemblyaihelper.h"

namespace {
int countPossiblePlays(Gameboard board, int player, int x, int y)
{
    Gameboard testboard = board;
    if(testboard.play(x,y,player))
    {
        int count = 0;
        for(int xi = 0; xi < 8; ++xi)
        {
            for(int yi = 0; yi < 8; ++yi)
            {
                if(testboard.play(xi,yi,player,true))
                {
                    ++count;
                }
            }
        }
        return count;
    }
    else
    {
        return -1;
    }
}
}

MovementCore::MovementCore()
{
}

bool MovementCore::retirement(Gameboard board, int player)
{
    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            if(board.play(x,y,player,true) && countPossiblePlays(board,player,x,y) < 5)
            {
                return false;
            }
        }
    }
    return true;
}

int MovementCore::mistrust(float const* const* const vote, Gameboard board, int player)
{
    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            if(vote[x][y] > 0 && board.play(x,y,player,true))
            {
                int count = countPossiblePlays(board,player,x,y);
                if(count == -1 || count == 0)
                {
                    return Core::_mistrustLarge;
                }
            }
        }
    }
    return Core::_noMistrust;
}

void MovementCore::propose(float ** const vote, Gameboard board, int player)
{
    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            if(board.play(x,y,player,true))
            {
                int count = countPossiblePlays(board,player,x,y);
                if(count != -1)
                {
                    vote[x][y] = 1 / (65 - count);
                }
            }
        }
    }
    AssemblyAI::ensureOnePossibleMove(vote,board,player);
}

void MovementCore::correct(float ** const vote, Gameboard board, int player)
{
    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            if(vote[x][y] > 0 && board.play(x,y,player,true))
            {
                switch(countPossiblePlays(board,player,x,y))
                {
                case -1:
                case 0:
                case 1:
                case 2:
                    vote[x][y] /= Core::_factorLarge;
                    break;
                case 3:
                case 4:
                case 5:
                    vote[x][y] /= Core::_factorSmall;
                    break;
                case 6:
                case 7:
                case 8:
                    break;
                case 9:
                case 10:
                case 11:
                    vote[x][y] *= Core::_factorSmall;
                    break;
                default:
                    vote[x][y] *= Core::_factorLarge;
                }
            }
        }
    }
}

QString MovementCore::name() const
{
    return "Movement Core";
}
