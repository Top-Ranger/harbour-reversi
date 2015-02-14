/*
  Copyright (C) 2014,2015 Marcus Soll
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

#include "rulehelper.h"
#include <QTime>

bool RuleHelper::isFrontierDisc(Gameboard board, int x, int y)
{
    if(board.owner(x,y) == 0)
    {
        return false;
    }

    for(int deltax=-1; deltax<=1; ++deltax)
    {
        for(int deltay=-1; deltay<=1; ++deltay)
        {
            if(deltax != 0 || deltay != 0)
            {
                if((x+deltax >= 0) && (y+deltay >= 0) && (x+deltax <= 7) && (y+deltay <= 7))
                {
                    if(board.owner(x+deltax,y+deltay) == 0)
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool RuleHelper::canTakeCorner(Gameboard board, int player)
{
    if(board.play(0,0,player,true))
    {
        return true;
    }
    else if(board.play(0,7,player,true))
    {
        return true;
    }
    else if(board.play(7,0,player,true))
    {
        return true;
    }
    else if(board.play(7,7,player,true))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool RuleHelper::canGetZeroDiscs(Gameboard board, int player)
{
    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            Gameboard testboard = board;
            if(testboard.play(x,y,opponent(player)))
            {
                if(testboard.points(player) == 0)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

RuleHelper::possibleMove RuleHelper::getPossibleTurn(Gameboard board, int player)
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
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
                possibleMove move;
                move.x = x;
                move.y = y;
                move.possible = true;
                return move;
            }

            y = (y+1)%8;
        }while(y != ystart);

        x = (x+1)%8;
    }while(x != xstart);

    possibleMove move;
    move.x = -1;
    move.y = -1;
    move.possible = false;
    return move;
}
