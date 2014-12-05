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

#include "edgecore.h"
#include "assemblyaihelper.h"

namespace {

int countEdgeTokens(Gameboard board, int player)
{
    // Edges are counted multiple times - that is a tradeoff we can live with
    int count = 0;

    for(int x = 0; x < 8; ++x)
    {
        if(board.owner(x,0) == player)
        {
            ++count;
        }
        if(board.owner(x,7) == player)
        {
            ++count;
        }
    }

    for(int y = 0; y < 8; ++y)
    {
        if(board.owner(0,y) == player)
        {
            ++count;
        }
        if(board.owner(7,y) == player)
        {
            ++count;
        }
    }
    return count;
}

bool canPlayEdge(Gameboard board, int player)
{
    for(int x = 0; x < 8; ++x)
    {
        if(board.play(x,0,player,true))
        {
            return true;
        }
        if(board.play(x,7,player,true))
        {
            return true;
        }
    }

    for(int y = 0; y < 8; ++y)
    {
        if(board.play(0,y,player,true))
        {
            return true;
        }
        if(board.play(7,y,player,true))
        {
            return true;
        }
    }
    return false;
}

} // End namespace

using AssemblyAI::opponent;

EdgeCore::EdgeCore()
{
}

bool EdgeCore::retirement(Gameboard board, int player)
{
    return !canPlayEdge(board,player);
}

int EdgeCore::mistrust(float const* const* const vote, Gameboard board, int player)
{
    int edgeOpponentNow = countEdgeTokens(board,opponent(player));

    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            if(vote[x][y] > 0 && board.play(x,y,player,true))
            {
                Gameboard testboard = board;
                testboard.play(x,y,player,false);
                for(int xi = 0; xi < 8; ++xi)
                {
                    for(int yi = 0; yi < 8; ++yi)
                    {
                        if(testboard.play(xi,yi,opponent(player),true))
                        {
                            Gameboard opponentBoard = testboard;
                            opponentBoard.play(xi,yi,opponent(player),false);
                            if((countEdgeTokens(opponentBoard,opponent(player)) - edgeOpponentNow) >= _boarderTokens)
                            {
                                return Core::_factorLarge;
                            }
                        }
                    }
                }
            }
        }
    }

    return Core::_noMistrust;
}

void EdgeCore::propose(float ** const vote, Gameboard board, int player)
{
    int edgeNow = countEdgeTokens(board,player);

    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            if(board.play(x,y,player,true))
            {
                Gameboard testboard = board;
                testboard.play(x,y,player,false);
                int edge = countEdgeTokens(testboard, player) - edgeNow;
                if(edge >= _boarderTokens)
                {
                    vote[x][y] = 1;
                }
                else if(edge > 0)
                {
                    vote[x][y] = 0.5;
                }
            }
        }
    }
    AssemblyAI::ensureOnePossibleMove(vote, board, player);
}

void EdgeCore::correct(float ** const vote, Gameboard board, int player)
{
    int edgeNow = countEdgeTokens(board,player);

    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            if(vote[x][y] > 0 && board.play(x,y,player,true))
            {
                Gameboard testboard = board;
                testboard.play(x,y,player,false);
                int edge = countEdgeTokens(testboard, player) - edgeNow;
                if(edge >= _boarderTokens)
                {
                    vote[x][y] *= Core::_factorLarge;
                }
                else if(edge > 0)
                {
                    vote[x][y] *= Core::_factorSmall;
                }
            }
        }
    }
}

QString EdgeCore::name() const
{
    return "Edge Core";
}
