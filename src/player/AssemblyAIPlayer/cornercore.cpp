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

#include "cornercore.h"
#include "assemblyaihelper.h"

using AssemblyAI::opponent;

CornerCore::CornerCore()
{
}

bool CornerCore::retirement(Gameboard board, int player)
{
    // The core should retrire when neither the player nor the opponent can get a corner
    return !(board.play(0,0,opponent(player),true) || board.play(7,0,opponent(player),true) || board.play(0,7,opponent(player),true) || board.play(7,7,opponent(player),true))
            && !(board.play(0,0,player,true) || board.play(7,0,player,true) || board.play(0,7,player,true) || board.play(7,7,player,true));
}

bool CornerCore::mistrust(float const* const* const vote, Gameboard board, int player)
{
    // Mistrust if you can get a corner but the proposal doesn't consider it

    if(!(vote[0][0] < 0) && board.play(0,0,player,true))
    {
        return true;
    }
    else if(!(vote[0][7] < 0) && board.play(0,7,player,true))
    {
        return true;
    }
    else if(!(vote[7][0] < 0) && board.play(7,0,player,true))
    {
        return true;
    }
    else if(!(vote[7][7] < 0) && board.play(7,7,player,true))
    {
        return true;
    }

    // Mistrust if a play is proposed where the opponent could get a corner
    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            if(vote[x][y] > 0 && ((x == 0 || x == 7) && (y == 0 || y == 7)) && board.play(x,y,player,true))
            {
                Gameboard testboard = board;
                testboard.play(x,y,player,false);
                if(!(testboard.play(0,0,opponent(player),true) || testboard.play(0,7,opponent(player),true) || testboard.play(7,0,opponent(player),true) || testboard.play(7,7,opponent(player),true)))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

void CornerCore::propose(float ** const vote, Gameboard board, int player)
{
    // Try to get corners
    if(board.play(0,0,player,true))
    {
        vote[0][0] = 1;
    }
    if(board.play(0,7,player,true))
    {
        vote[0][7] = 1;
    }
    if(board.play(7,0,player,true))
    {
        vote[7][0] = 1;
    }
    if(board.play(7,7,player,true))
    {
        vote[7][7] = 1;
    }

    // Propose plays where the opponent can't get a corner
    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            if(vote[x][y] > 0 && ((x == 0 || x == 7) && (y == 0 || y == 7)) && board.play(x,y,player,true))
            {
                Gameboard testboard = board;
                testboard.play(x,y,opponent(player),false);
                if(!(testboard.play(0,0,opponent(player),true) || testboard.play(0,7,opponent(player),true) || testboard.play(7,0,opponent(player),true) || testboard.play(7,7,opponent(player),true)))
                {
                    vote[x][y] = 0.1;
                }
            }
        }
    }
    AssemblyAI::ensureOnePossibleMove(vote,board,player);
}

void CornerCore::correct(float ** const vote, Gameboard board, int player)
{
    // Give Corners a high priority
    vote[0][0] *= Core::_factorLarge;
    vote[0][7] *= Core::_factorLarge;
    vote[7][0] *= Core::_factorLarge;
    vote[7][7] *= Core::_factorLarge;

    // Give plays where an opponent can get a corner a low priority
    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            if(vote[x][y] > 0 && ((x == 0 || x == 7) && (y == 0 || y == 7)) && board.play(x,y,player,true))
            {
                Gameboard testboard = board;
                testboard.play(x,y,opponent(player),false);
                if(testboard.play(0,0,opponent(player),true) || testboard.play(0,7,opponent(player),true) || testboard.play(7,0,opponent(player),true) || testboard.play(7,7,opponent(player),true))
                {
                    vote[x][y] /= Core::_factorLarge;
                }
            }
        }
    }
}

QString CornerCore::name() const
{
    return "Corner Core";
}
