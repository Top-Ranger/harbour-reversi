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

#include "controlaiplayer.h"
#include <QTime>
#include <limits>

ControlAIPlayer::ControlAIPlayer(QObject *parent) :
    Player(parent),
    _availableFunctions(),
    _currentFunctions(),
    _priority()
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    // Fill _availableFunctions
    _availableFunctions[0] = &ControlAIPlayer::functionFindSleepers;
    _availableFunctions[1] = &ControlAIPlayer::functionControlArea;
    _availableFunctions[2] = &ControlAIPlayer::functionEdge;
    _availableFunctions[3] = &ControlAIPlayer::functionAvoidCluster;

    // Fill _currentFunctions
    _currentFunctions[0] = _availableFunctions[qrand() % _sizeAvailableFunctions];
    _currentFunctions[1] = _availableFunctions[qrand() % _sizeAvailableFunctions];
    _currentFunctions[2] = _availableFunctions[qrand() % _sizeAvailableFunctions];
}

bool ControlAIPlayer::isHuman()
{
    return false;
}

void ControlAIPlayer::doTurn(Gameboard board, int player)
{
    // Change function
    _currentFunctions[qrand() % _sizeCurrentFunctions] = _availableFunctions[qrand() % _sizeAvailableFunctions];

    // Initialising _priority
    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            if(((x == 0) || (x == 7)) && ((y == 0) || (y == 7)))
            {
                _priority[x][y] = 150;
            }
            else if(((x <= 1) || (x >= 6)) && ((y <= 1) || (y >= 6)))
            {
                _priority[x][y] = -20;
            }
            else
            {
                _priority[x][y] = 0;
            }
        }
    }

    // Do actual turn

    int max = std::numeric_limits<int>::min();
    int x = qrand()%8;
    int y = qrand()%8;
    int xstart = x;
    int ystart = y;
    int xmax = -1;
    int ymax = -1;
    int xmaxbad = -1;
    int ymaxbad = -1;
    int maxbad = std::numeric_limits<int>::min();
    Gameboard testboard;

    for(int i = 0; i < _sizeCurrentFunctions; ++i)
    {
        (this->*ControlAIPlayer::_currentFunctions[i])(board,player);
    }

    do
    {
        do
        {
            testboard = board;

            if(testboard.play(x,y,player))
            {
                int temp = 0;
                for(int tempx = 0; tempx < 8; ++tempx)
                {
                    for(int tempy = 0; tempy < 8; ++tempy)
                    {
                        if(testboard.owner(tempx,tempy) == player)
                        {
                            temp += _priority[tempx][tempy];
                        }
                    }
                }
                if(filter(x,y,testboard,player))
                {
                    if(temp > max)
                    {
                        max = temp;
                        xmax = x;
                        ymax = y;
                    }
                }
                else
                {
                    if(temp > maxbad)
                    {
                        maxbad = temp;
                        xmaxbad = x;
                        ymaxbad = y;
                    }
                }
            }

            y = (y+1)%8;
        }while(y != ystart);

        x = (x+1)%8;
    }while(x != xstart);

    if((xmax != -1) && (ymax != -1))
    {
        emit turn(xmax, ymax);
    }
    else
    {
        emit turn(xmaxbad, ymaxbad);
    }
}

void ControlAIPlayer::humanInput(int x, int y)
{
}

int inline ControlAIPlayer::opponent(int player)
{
    return player==1?2:1;
}

bool ControlAIPlayer::filter(int x, int y, Gameboard board, int player)
{
    // This function should the AI prevent from doing stupid things
    // Prevent the opponent from playing a sleeper
    if(!(((x == 0) || (x == 7)) && ((y == 0) || (y == 7))))
    {
        if((x == 0) || (x == 7))
        {
            if(y != 0 && board.owner(x,y-1) == 0)
            {
                int tempy = y-2;
                {
                    while(tempy >= 0)
                    {
                        if(board.owner(x,tempy) == player)
                        {
                            return false;
                        }
                        else if(board.owner(x,tempy) == 0)
                        {
                            break;
                        }
                        --tempy;
                    }
                }
            }
            if(y != 7 && board.owner(x,y+1) == 0)
            {
                int tempy = y+2;
                {
                    while(tempy <= 7)
                    {
                        if(board.owner(x,tempy) == player)
                        {
                            return false;
                        }
                        else if(board.owner(x,tempy) == 0)
                        {
                            break;
                        }
                        ++tempy;
                    }
                }
            }
        }
        else if((y == 0) || (y == 7))
        {
            if(x != 0 && board.owner(x-1,y) == 0)
            {
                int tempx = x-2;
                {
                    while(tempx >= 0)
                    {
                        if(board.owner(tempx,y) == player)
                        {
                            return false;
                        }
                        else if(board.owner(tempx,y) == 0)
                        {
                            break;
                        }
                        --tempx;
                    }
                }
            }
            if(x != 7 && board.owner(x+1,y) == 0)
            {
                int tempx = x+2;
                {
                    while(tempx <= 7)
                    {
                        if(board.owner(tempx,y) == player)
                        {
                            return false;
                        }
                        else if(board.owner(tempx,y) == 0)
                        {
                            break;
                        }
                        ++tempx;
                    }
                }
            }
        }
    }
    // Prevent the opponent from getting a corner
    if(board.play(0,0,opponent(player),true) || board.play(0,7,opponent(player),true) || board.play(7,0,opponent(player),true) || board.play(7,7,opponent(player),true))
    {
        return false;
    }
    return true;
}

void ControlAIPlayer::functionFindSleepers(Gameboard board, int player)
{
    // This function tries to find places where you can get a disc which is surrounded by opponent discs

    // 1. To only add points ONCE we need a copy

    int oldPriority[8][8];
    for(int i = 0; i < 8; ++i)
    {
        for(int j = 0; j < 8; ++j)
        {
            oldPriority[i][j] = _priority[i][j];
        }
    }

    // 2. On the outside of the board

    for(int x = 0; x < 8; x += 7)
    {
        for(int y = 1; y < 7; ++y)
        {
            if(board.owner(x,y) == player || board.owner(x,y) == 0)
            {
                bool test = true;
                bool result = true;
                bool onlyOwn = true;
                int testy = y;
                while(test && testy > 0)
                {
                    --testy;
                    if(board.owner(x,testy) == 0)
                    {
                        test = false;
                        result = !onlyOwn;
                    }
                    else if(board.owner(x,testy) == opponent(player))
                    {
                        onlyOwn = false;
                    }
                    else
                    {
                        if(!onlyOwn)
                        {
                            test = false;
                            result = false;
                        }
                    }
                }

                testy = y;
                test = result;
                onlyOwn = true;

                while(test && testy < 7)
                {
                    ++testy;
                    if(board.owner(x,testy) == 0)
                    {
                        test = false;
                        result = !onlyOwn;
                    }
                    else if(board.owner(x,testy) == opponent(player))
                    {
                        onlyOwn = false;
                    }
                    else
                    {
                        if(!onlyOwn)
                        {
                            test = false;
                            result = false;
                        }
                    }
                }
                if(result)
                {
                    if(!(_priority[x][y] > oldPriority[x][y]))
                    {
                        _priority[x][y] += 5;
                    }

                    test = true;
                    for(testy = y-1; test && testy >= 0; --testy)
                    {
                        if(board.owner(x,testy) == 0)
                        {
                            test = false;
                        }
                        else if(board.owner(x,testy) == opponent(player))
                        {
                            if(!(_priority[x][testy] < oldPriority[x][testy]))
                            {
                                _priority[x][testy] -= 5;
                            }
                        }
                    }

                    test = true;
                    for(testy = y+1; test && testy < 8; ++testy)
                    {
                        if(board.owner(x,testy) == 0)
                        {
                            test = false;
                        }
                        else if(board.owner(x,testy) == opponent(player))
                        {
                            if(!(_priority[x][testy] < oldPriority[x][testy]))
                            {
                                _priority[x][testy] -= 5;
                            }
                        }
                    }
                }
            }
        }
    }

    for(int y = 0; y < 8; y += 7)
    {
        for(int x = 1; x < 7; ++x)
        {
            if(board.owner(x,y) == player || board.owner(x,y) == 0)
            {
                bool test = true;
                bool result = true;
                bool onlyOwn = true;
                int testx = x;
                while(test && testx > 0)
                {
                    --testx;
                    if(board.owner(testx,y) == 0)
                    {
                        test = false;
                        result = !onlyOwn;
                    }
                    else if(board.owner(testx,y) == opponent(player))
                    {
                        onlyOwn = false;
                    }
                    else
                    {
                        if(!onlyOwn)
                        {
                            test = false;
                            result = false;
                        }
                    }
                }

                testx = x;
                test = result;
                onlyOwn = true;

                while(test && testx < 7)
                {
                    ++testx;
                    if(board.owner(testx,y) == 0)
                    {
                        test = false;
                        result = !onlyOwn;
                    }
                    else if(board.owner(testx,y) == opponent(player))
                    {
                        onlyOwn = false;
                    }
                    else
                    {
                        if(!onlyOwn)
                        {
                            test = false;
                            result = false;
                        }
                    }
                }
                if(result)
                {
                    if(!(_priority[x][y] > oldPriority[x][y]))
                    {
                        _priority[x][y] += 5;
                    }

                    test = true;
                    for(testx = x-1; test && testx >= 0; --testx)
                    {
                        if(board.owner(testx,y) == 0)
                        {
                            test = false;
                        }
                        else if(board.owner(testx,y) == opponent(player))
                        {
                            if(!(_priority[testx][y] < oldPriority[testx][y]))
                            {
                                _priority[testx][y] -= 5;
                            }
                        }
                    }

                    test = true;
                    for(testx = x+1; test && testx < 8; ++testx)
                    {
                        if(board.owner(testx,y) == 0)
                        {
                            test = false;
                        }
                        else if(board.owner(testx,y) == opponent(player))
                        {
                            if(!(_priority[testx][y] < oldPriority[testx][y]))
                            {
                                _priority[testx][y] -= 5;
                            }
                        }
                    }
                }
            }
        }
    }
}

void ControlAIPlayer::functionControlArea(Gameboard board, int player)
{
    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
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
            _priority[x][y] += (result/2);
        }
    }
}

void ControlAIPlayer::functionEdge(Gameboard board, int player)
{
    for(int i = 0; i < 8; ++i)
    {
        _priority[i][0] += 2;
        _priority[i][7] += 2;
        _priority[0][i] += 2;
        _priority[0][i] += 2;
    }
}

void ControlAIPlayer::functionAvoidCluster(Gameboard board, int player)
{
    // Initialize temp priority - so we can give a min priority
    int newPriority[8][8];
    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            newPriority[x][y] = 0;
        }
    }

    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            // Find all own cluster
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
                        if(!(board.owner(tempx,tempy) == player))
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
                        if(!(board.owner(tempx,tempy) == player))
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
                        if(!(board.owner(tempx,tempy) == player))
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
                        if(!(board.owner(tempx,tempy) == player))
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

            if(result != 0)
            {

                // Now set all priority
                ++result;
                int tempx = x-result;
                int tempy = y-result;

                while(tempx <= (x+result))
                {
                    if((tempx >= 0) && (tempx <= 7) && (tempy >= 0) && (tempy <= 7))
                    {
                        if(newPriority[x][y] > -4)
                        {
                            --newPriority[x][y];
                        }
                    }
                    ++tempx;
                }
                --tempx;

                while(test && (x+result))
                {
                    if((tempx >= 0) && (tempx <= 7) && (tempy >= 0) && (tempy <= 7))
                    {
                        if(newPriority[x][y] > -4)
                        {
                            --newPriority[x][y];
                        }
                    }
                    ++tempy;
                }
                --tempy;

                while(test && (x-result))
                {
                    if((tempx >= 0) && (tempx <= 7) && (tempy >= 0) && (tempy <= 7))
                    {
                        if(newPriority[x][y] > -4)
                        {
                            --newPriority[x][y];
                        }
                    }
                    --tempx;
                }
                ++tempx;

                while(test && (x-result))
                {
                    if((tempx >= 0) && (tempx <= 7) && (tempy >= 0) && (tempy <= 7))
                    {
                        if(newPriority[x][y] > -4)
                        {
                            --newPriority[x][y];
                        }
                    }
                    --tempy;
                }
            }
        }
    }
    // Now add the priority
    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            _priority[x][y] += newPriority[x][y];
        }
    }
}
