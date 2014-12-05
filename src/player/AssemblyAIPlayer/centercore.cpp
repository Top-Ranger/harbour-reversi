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

#include "centercore.h"
#include "assemblyaihelper.h"

CenterCore::CenterCore()
{
}

bool CenterCore::retirement(Gameboard board, int player)
{
    int count = 0;
    for(int x = 3; x <=4; ++x)
    {
        for(int y = 3; y <= 4; ++y)
        {
            if(board.owner(x,y) == player)
            {
                ++count;
            }
        }
    }
    return count >= 2;
}

int CenterCore::mistrust(float const* const* const vote, Gameboard board, int player)
{
    bool noDisc = false;
    int count = 0;
    for(int x = 0; x < 8 && !noDisc; ++x)
    {
        for(int y = 0; y < 8 && !noDisc; ++y)
        {
            if(vote[x][y] > 0 && board.play(x,y,player,true))
            {
                count = 0;
                Gameboard testboard = board;
                testboard.play(x,y,player,false);
                for(int xi = 2; xi <=5; ++xi)
                {
                    for(int yi = 2; yi <= 5; ++yi)
                    {
                        if(testboard.owner(xi,yi) == player)
                        {
                            ++count;
                        }
                    }
                }
                noDisc = count == 0;
            }
        }
    }
    if(!noDisc)
    {
        return Core::_mistrustLarge;
    }
    else
    {
        return Core::_noMistrust;
    }
}

void CenterCore::propose(float ** const vote, Gameboard board, int player)
{
    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            if(board.play(x,y,player,true))
            {
                Gameboard testboard = board;
                testboard.play(x,y,player,false);
                int count = 0;
                for(int x_center = 3; x_center <=4; ++x_center)
                {
                    for(int y_center = 3; y_center <= 4; ++y_center)
                    {
                        if(testboard.owner(x_center,y_center) == player)
                        {
                            ++count;
                        }
                    }
                }
                if(count >= 4)
                {
                    vote[x][y] = 1;
                }
                else if(count > 0)
                {
                    vote[x][y] = 1 / (5-count);
                }
            }
        }
    }
    AssemblyAI::ensureOnePossibleMove(vote,board,player);
}

void CenterCore::correct(float ** const vote, Gameboard board, int player)
{
    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            if(vote[x][y] > 0 && board.play(x,y,player,true))
            {
                Gameboard testboard = board;
                testboard.play(x,y,player,false);
                int count = 0;
                for(int x_center = 3; x_center <=4; ++x_center)
                {
                    for(int y_center = 3; y_center <= 4; ++y_center)
                    {
                        if(testboard.owner(x_center,y_center) == player)
                        {
                            ++count;
                        }
                    }
                }
                switch(count)
                {
                case 0:
                    vote[x][y] /= Core::_factorLarge;
                    break;
                case 1:
                    vote[x][y] /= Core::_factorSmall;
                    break;
                case 3:
                    vote[x][y] *= Core::_factorSmall;
                    break;
                case 4:
                    vote[x][y] *= Core::_factorLarge;
                    break;
                default:
                    break;
                }
            }
        }
    }
}

QString CenterCore::name() const
{
    return "Center Core";
}
