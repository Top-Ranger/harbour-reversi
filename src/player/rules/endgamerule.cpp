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

#include "endgamerule.h"
#include "greedyrule.h"
#include "rulehelper.h"

using RuleHelper::opponent;

const int EndgameRule::_borderDiscs;

namespace {
int amountDiscsCanGet(Gameboard board, int player, int x_orig, int y_orig)
{
    int count = 0;

    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            if(board.owner(x,y) == player)
            {
                --count;
            }
        }
    }

    board.play(x_orig,y_orig,player);
    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            if(board.owner(x,y) == player)
            {
                ++count;
            }
        }
    }
    return count;
}
}

EndgameRule::EndgameRule(QObject *parent) :
    Rule(parent)
{
}

bool EndgameRule::applicable(Gameboard board, int player)
{
    return (board.points(player) + board.points(opponent(player))) >= _borderDiscs;
}

void EndgameRule::doTurn(Gameboard board, int player)
{
    // Find plays where the opponent can't do anything after that
    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            Gameboard temp = board;
            if(temp.play(x,y,player) && !temp.isTurnPossible(opponent(player)))
            {
                emit turn(x,y);
                return;
            }
        }
    }

    //Get the most discs while letting the opponent get few discs
    int max = -1000;
    int x_turn = -1;
    int y_turn = -1;
    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            if(board.play(x,y,player,true))
            {
                int count = amountDiscsCanGet(board,player,x,y);
                Gameboard testboard = board;
                testboard.play(x,y,player,false);
                int countOpponent = -1;
                for(int xi = 0; xi < 8; ++xi)
                {
                    for(int yi = 0; yi < 8; ++yi)
                    {
                        if(testboard.play(xi,yi,opponent(player),true))
                        {
                            int temp = amountDiscsCanGet(testboard,opponent(player),xi,yi);
                            if(temp > countOpponent)
                            {
                                countOpponent = temp;
                            }
                        }
                    }
                }
                count -= countOpponent;
                if(count > max)
                {
                    max = count;
                    x_turn = x;
                    y_turn = y;
                }
            }
        }
    }

    if(!(max == -1000 || x_turn == -1 || y_turn == -1))
    {
        emit turn(x_turn,y_turn);
        return;
    }

    //If everything fails do a greedy rule
    GreedyRule fallback;
    QObject::connect(&fallback,SIGNAL(turn(int,int)),this,SLOT(getTurn(int,int)));
    fallback.doTurn(board,player);
    QObject::disconnect(&fallback,SIGNAL(turn(int,int)),this,SLOT(getTurn(int,int)));

}

QString EndgameRule::name()
{
    return tr("Endgame Rule");
}

void EndgameRule::getTurn(int x, int y)
{
    emit turn(x,y);
}
