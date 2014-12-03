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

#include "adaptivetreeaiplayer.h"
#include <QTime>
#include <limits>

AdaptiveTreeAIPlayer::AdaptiveTreeAIPlayer(QObject *parent) :
    Player(parent),
    _firstBoard(true),
    _fewerFrontierDiscs(1),
    _fewerMoves(1),
    _moreOwnMoves(1),
    _greedy(1),
    _edge(1),
    _lonely(1),
    _oldBoard()
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
}

bool AdaptiveTreeAIPlayer::isHuman()
{
    return false;
}

void AdaptiveTreeAIPlayer::doTurn(Gameboard board, int player)
{
    float max = -1048576;
    int x = qrand()%8;
    int y = qrand()%8;
    int xstart = x;
    int ystart = y;
    int xmax = -1;
    int ymax = -1;
    Gameboard testboard;
    int composure = qrand() % _composure;

    if(!_firstBoard)
    {
        for(int opponentX = 0; opponentX < 8; ++opponentX)
        {
            for(int opponentY = 0; opponentY < 8; ++opponentY)
            {
                if(_oldBoard.owner(opponentX,opponentY) == 0 && board.owner(opponentX,opponentY) == opponent(player))
                {
                    adjustFactors(_oldBoard,opponentX,opponentY, opponent(player));
                }
            }
        }
        _oldBoard = board;
    }
    else
    {
        _oldBoard = board;
        _firstBoard = false;
    }
    do
    {
        do
        {
            testboard = board;

            if(testboard.play(x,y,player))
            {
                if(composure == 0)
                {
                    if(testboard.points(player) > max)
                    {
                        max = testboard.points(player);
                        xmax = x;
                        ymax = y;
                    }
                }
                else
                {
                    float temp = buildTree(testboard, opponent(player), player, board.points(player), board.points(opponent(player)), _depth);
                    if(temp > max)
                    {
                        max = temp;
                        xmax = x;
                        ymax = y;
                    }
                }
            }

            y = (y+1)%8;
        }while(y != ystart);

        x = (x+1)%8;
    }while(x != xstart);

    if(composure == 0)
    {
        emit sendMessage(QString(tr("Let's get %1:%2! NOW!")).arg(xmax).arg(ymax));
    }
    else
    {
        emit sendMessage(QString(tr("If you do that... I'll do %1:%2, for %3 points")).arg(xmax).arg(ymax).arg(max));
    }
    emit turn(xmax,ymax);
}

void AdaptiveTreeAIPlayer::humanInput(int x, int y)
{
}

float AdaptiveTreeAIPlayer::buildTree(Gameboard board, int player, int I, int old, int opponentOld, int depth)
{
    --depth;
    int n = 0;
    float advantage = 0;

    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            if(board.play(x,y,player,true))
            {
                if(depth <= 0)
                {
                    if(player == I)
                    {
                        advantage += _valueMove;
                    }
                    else
                    {
                        advantage -= _valueMove;
                    }
                    Gameboard newBoard = board;
                    newBoard.play(x,y,player,false);
                    if(player == I)
                    {
                        advantage += calculateScore(newBoard, I,newBoard.points(I)-old, newBoard.points(opponent(I))-opponentOld);
                    }
                    else
                    {
                        advantage += calculateScore(newBoard, I, newBoard.points(I)-old, newBoard.points(opponent(I))-opponentOld)  * calculateFactor(board,x,y,opponent(I));
                    }
                    ++n;
                }
                else
                {
                    if(player == I)
                    {
                        advantage += _valueMove;
                    }
                    else
                    {
                        advantage -= _valueMove;
                    }
                    Gameboard newBoard = board;
                    newBoard.play(x,y,player,false);
                    if(player == I)
                    {
                        advantage += buildTree(newBoard,opponent(player),I,old,opponentOld,depth);
                    }
                    else
                    {
                        advantage += buildTree(newBoard,opponent(player),I,old,opponentOld,depth) * calculateFactor(board,x,y,opponent(I));
                    }
                    ++n;
                }
            }
        }
    }
    if(n == 0)
    {
        if(board.points(I) > board.points(opponent(I)))
        {
            return _valueFullBoard;
        }
        else
        {
            return -_valueFullBoard;
        }
    }
    else
    {
        return advantage / n;
    }
}

int inline AdaptiveTreeAIPlayer::opponent(int player)
{
    return player==1?2:1;
}

float AdaptiveTreeAIPlayer::calculateScore(Gameboard board, int I, int change, int opponentChange)
{
    float score = 0;

    score += change * _factorChange;
    score -= opponentChange * _factorOpponentChange;

    if(board.points(I) > board.points(opponent(I)))
    {
        score += _factorRatio * (board.points(I)/(board.points(opponent(I))+1));
    }
    else if(board.points(I) < board.points(opponent(I)))
    {
        score -= _factorRatio * (board.points(I)/(board.points(opponent(I))+1));
    }

    if(board.owner(0,0) == I)
    {
        score += _valueCorner;
    }
    else if(board.owner(0,0) == opponent(I))
    {
        score -= _valueCorner;
    }
    else if(board.owner(0,0) == 0)
    {
        if(board.owner(1,1) == I)
        {
            score -= _valueCorner;
        }
        else if(board.owner(1,1) == opponent(I))
        {
            score += _valueCorner;
        }
    }

    if(board.owner(7,0) == I)
    {
        score += _valueCorner;
    }
    else if(board.owner(7,0) == opponent(I))
    {
        score -= _valueCorner;
    }
    else if(board.owner(7,0) == 0)
    {
        if(board.owner(6,1) == I)
        {
            score -= _valueCorner;
        }
        else if(board.owner(6,1) == opponent(I))
        {
            score += _valueCorner;
        }
    }

    if(board.owner(0,7) == I)
    {
        score += _valueCorner;
    }
    else if(board.owner(0,7) == opponent(I))
    {
        score -= _valueCorner;
    }
    else if(board.owner(0,7) == 0)
    {
        if(board.owner(1,6) == I)
        {
            score -= _valueCorner;
        }
        else if(board.owner(1,6) == opponent(I))
        {
            score += _valueCorner;
        }
    }

    if(board.owner(7,7) == I)
    {
        score += _valueCorner;
    }
    else if(board.owner(7,7) == opponent(I))
    {
        score -= _valueCorner;
    }
    else if(board.owner(7,7) == 0)
    {
        if(board.owner(6,6) == I)
        {
            score -= _valueCorner;
        }
        else if(board.owner(6,6) == opponent(I))
        {
            score += _valueCorner;
        }
    }

    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; y++)
        {
            int valueStone;

            if(x == 7 || x == 0 || y == 7 || y == 0)
            {
                valueStone = 7;
            }
            else if(x == 6 || x == 1 || y == 6 || y == 1)
            {
                valueStone = 3;
            }
            else if((x == 3 || x == 4) && (y == 3 && y == 4))
            {
                valueStone = 5;
            }
            else
            {
                valueStone = 1;
            }

            if(board.owner(x,y) == I)
            {
                score += _factorStone * valueStone;
            }
            else if(board.owner(x,y) == opponent(I))
            {
                score -= _factorStone * valueStone;
            }
        }
    }
    if(!board.isTurnPossible(opponent(I)))
    {
        if(board.points(I) > board.points(opponent(I)))
        {
            score += _valueFullBoard;
        }
        else
        {
            score += -_valueFullBoard;
        }
    }
    return score;
}

bool AdaptiveTreeAIPlayer::triesFewerFrontierDiscs(Gameboard board, int x, int y, int opponentValue)
{
    if(board.owner(x,y) != 0)
    {
        return false;
    }

    int n = 0;
    int frontierDiscs = 0;
    int minFrontierDiscs = 64;

    for(int Xvar = 0; Xvar < 8; ++Xvar)
    {
        for(int Yvar = 0; Yvar < 8; ++Yvar)
        {
            Gameboard testboard = board;
            int currentFrontierDiscs = 0;

            if(testboard.play(Xvar,Yvar,opponentValue))
            {
                for(int newX = 0; newX < 8; ++newX)
                {
                    for(int newY = 0; newY < 8; ++newY)
                    {
                        if(testboard.owner(newX,newY) == opponentValue && isFrontierDisc(testboard,newX,newY))
                        {
                            ++frontierDiscs;
                            ++currentFrontierDiscs;
                        }
                    }
                }
                if(currentFrontierDiscs < minFrontierDiscs)
                {
                    minFrontierDiscs = currentFrontierDiscs;
                }
                ++n;
            }
        }
    }

    if(n == 0)
    {
        return false;
    }
    else
    {
        if(board.play(x,y,opponentValue))
        {
            int realFrontierDiscs = 0;

            for(int newX = 0; newX < 8; ++newX)
            {
                for(int newY = 0; newY < 8; ++newY)
                {
                    if(board.owner(newX,newY) == opponentValue && isFrontierDisc(board,newX,newY))
                    {
                        ++realFrontierDiscs;
                    }
                }
            }

            return realFrontierDiscs < (minFrontierDiscs + (frontierDiscs/(n*2)));
        }
        else
        {
            return false;
        }
    }
}

bool AdaptiveTreeAIPlayer::triesFewerMoves(Gameboard board, int x, int y, int opponentValue)
{
    if(board.owner(x,y) != 0)
    {
        return false;
    }

    int n = 0;
    int moves = 0;
    int minMoves = 64;

    for(int Xvar = 0; Xvar < 8; ++Xvar)
    {
        for(int Yvar = 0; Yvar < 8; ++Yvar)
        {
            Gameboard testboard = board;
            int currentMoves = 0;

            if(testboard.play(Xvar,Yvar,opponentValue))
            {
                for(int newX = 0; newX < 8; ++newX)
                {
                    for(int newY = 0; newY < 8; ++newY)
                    {
                        if(testboard.play(newX,newY,opponent(opponentValue),true))
                        {
                            ++moves;
                            ++currentMoves;
                        }
                    }
                }
                if(currentMoves < minMoves)
                {
                    minMoves = currentMoves;
                }
                ++n;
            }
        }
    }

    if(n == 0)
    {
        return false;
    }
    else
    {
        if(board.play(x,y,opponentValue))
        {
            int realMoves = 0;

            for(int newX = 0; newX < 8; ++newX)
            {
                for(int newY = 0; newY < 8; ++newY)
                {
                    if(board.play(newX,newY,opponent(opponentValue),true))
                    {
                        ++realMoves;
                    }
                }
            }

            return realMoves < (minMoves + (moves/(n*2)));
        }
        else
        {
            return false;
        }
    }
}

bool AdaptiveTreeAIPlayer::triesMoreOwnMoves(Gameboard board, int x, int y, int opponentValue)
{
    if(board.owner(x,y) != 0)
    {
        return false;
    }

    int n = 0;
    int moves = 0;
    int maxMoves = 0;

    for(int Xvar = 0; Xvar < 8; ++Xvar)
    {
        for(int Yvar = 0; Yvar < 8; ++Yvar)
        {
            Gameboard testboard = board;
            int currentMoves = 0;

            if(testboard.play(Xvar,Yvar,opponentValue))
            {
                for(int newX = 0; newX < 8; ++newX)
                {
                    for(int newY = 0; newY < 8; ++newY)
                    {
                        if(testboard.play(newX,newY,opponentValue,true))
                        {
                            ++moves;
                            ++currentMoves;
                        }
                    }
                }
                if(currentMoves > maxMoves)
                {
                    maxMoves = currentMoves;
                }
                ++n;
            }
        }
    }

    if(n == 0)
    {
        return false;
    }
    else
    {
        if(board.play(x,y,opponentValue))
        {
            int realMoves = 0;

            for(int newX = 0; newX < 8; ++newX)
            {
                for(int newY = 0; newY < 8; ++newY)
                {
                    if(board.play(newX,newY,opponentValue,true))
                    {
                        ++realMoves;
                    }
                }
            }

            return realMoves > (maxMoves - (moves/(n*2)));
        }
        else
        {
            return false;
        }
    }
}

bool AdaptiveTreeAIPlayer::triesGreedy(Gameboard board, int x, int y, int opponentValue)
{
    if(board.owner(x,y) != 0)
    {
        return false;
    }

    int n = 0;
    int discs = 0;
    int maxDiscs = 0;

    for(int Xvar = 0; Xvar < 8; ++Xvar)
    {
        for(int Yvar = 0; Yvar < 8; ++Yvar)
        {
            Gameboard testboard = board;
            int currentDiscs = 0;

            if(testboard.play(Xvar,Yvar,opponentValue))
            {
                for(int newX = 0; newX < 8; ++newX)
                {
                    for(int newY = 0; newY < 8; ++newY)
                    {
                        if(testboard.owner(newX,newY) == opponentValue)
                        {
                            ++discs;
                            ++currentDiscs;
                        }
                    }
                }
                if(currentDiscs > maxDiscs)
                {
                    maxDiscs = currentDiscs;
                }
                ++n;
            }
        }
    }

    if(n == 0)
    {
        return false;
    }
    else
    {
        if(board.play(x,y,opponentValue))
        {
            int realDiscs = 0;

            for(int newX = 0; newX < 8; ++newX)
            {
                for(int newY = 0; newY < 8; ++newY)
                {
                    if(board.owner(newX,newY) == opponentValue)
                    {
                        ++realDiscs;
                    }
                }
            }

            return realDiscs > (maxDiscs - (discs/(n*2)));
        }
        else
        {
            return false;
        }
    }
}

bool AdaptiveTreeAIPlayer::triesEdge(Gameboard board, int x, int y, int opponentValue)
{
    if(x != 7 && x != 0 && y != 7  && y != 0)
    {
        return false;
    }

    if(board.owner(x,y) != 0)
    {
        return false;
    }

    int n = 0;
    int discs = 0;
    int maxDiscs = 0;

    for(int Xvar = 0; Xvar < 8; Xvar+=7)
    {
        for(int Yvar = 0; Yvar < 8; Yvar+=7)
        {
            Gameboard testboard = board;
            int currentDiscs = 0;

            if(testboard.play(Xvar,Yvar,opponentValue))
            {
                for(int newX = 0; newX < 8; ++newX)
                {
                    if(testboard.owner(newX,0) == opponentValue)
                    {
                        ++discs;
                        ++currentDiscs;
                    }
                    if(testboard.owner(newX,7) == opponentValue)
                    {
                        ++discs;
                        ++currentDiscs;
                    }
                }

                for(int newY = 0; newY < 8; ++newY)
                {
                    if(testboard.owner(0,newY) == opponentValue)
                    {
                        ++discs;
                        ++currentDiscs;
                    }
                    if(testboard.owner(7,newY) == opponentValue)
                    {
                        ++discs;
                        ++currentDiscs;
                    }
                }

                if(currentDiscs > maxDiscs)
                {
                    maxDiscs = currentDiscs;
                }
                ++n;
            }
        }
    }

    if(n == 0)
    {
        return false;
    }
    else
    {
        int realDiscs = 0;

        if(board.play(x,y,opponentValue))
        {
            for(int newX = 0; newX < 8; ++newX)
            {
                if(board.owner(newX,0) == opponentValue)
                {
                    ++realDiscs;
                }
                if(board.owner(newX,7) == opponentValue)
                {
                    ++realDiscs;
                }
            }

            for(int newY = 0; newY < 8; ++newY)
            {
                if(board.owner(0,newY) == opponentValue)
                {
                    ++realDiscs;
                }
                if(board.owner(7,newY) == opponentValue)
                {
                    ++realDiscs;
                }
            }

            return realDiscs >= (maxDiscs - (discs/(n*2)));
        }
        else
        {
            return false;
        }
    }
}

bool AdaptiveTreeAIPlayer::triesLonely(Gameboard board, int x, int y, int opponentValue)
{
    if(board.owner(x,y) != 0)
    {
        return false;
    }

    int n = 0;

    for(int deltax=-2; deltax<=2; ++deltax)
    {
        for(int deltay=-2; deltay<=2; ++deltay)
        {
            if(deltax != 0 || deltay != 0)
            {
                int newX = x + deltax;
                int newY = y + deltay;

                if(newX >= 0 && newX < 8 && newY >= 0 && newY < 8)
                {
                    if(board.owner(newX,newY) == opponentValue)
                    {
                        ++n;
                        if(n >= _borderLonely)
                        {
                            return false;
                        }
                    }
                }
            }
        }
    }

    return true;
}

void AdaptiveTreeAIPlayer::adjustFactors(Gameboard board, int x, int y, int opponentValue)
{
    if(triesFewerFrontierDiscs(board,x,y,opponentValue))
    {
        _fewerFrontierDiscs *= _factorLearnung;
    }
    else
    {
        _fewerFrontierDiscs /= _factorLearnung;
    }

    if(triesFewerMoves(board,x,y,opponentValue))
    {
        _fewerMoves *= _factorLearnung;
    }
    else
    {
        _fewerMoves /= _factorLearnung;
    }

    if(triesMoreOwnMoves(board,x,y,opponentValue))
    {
        _moreOwnMoves *= _factorLearnung;
    }
    else
    {
        _moreOwnMoves /= _factorLearnung;
    }

    if(triesGreedy(board,x,y,opponentValue))
    {
        _greedy *= _factorLearnung;
    }
    else
    {
        _greedy /= _factorLearnung;
    }

    if(triesEdge(board,x,y,opponentValue))
    {
        _edge *= (_factorLearnung * 1.5);
    }
    else
    {
        _edge /= (_factorLearnung * 1.5);
    }

    if(triesLonely(board,x,y,opponentValue))
    {
        _lonely *= (_factorLearnung * 1.5);
    }
    else
    {
        // Because there are only a few chances to actually play lonely the negative effect
        // should only be applied if he don't use an opportiunity

        bool test = true;

        for(int newX = 0; newX < 8 && test; ++newX)
        {
            for(int newY = 0; newY < 8; ++newY)
            {
                if(board.play(newX,newY,opponentValue,true) && triesLonely(board,x,y,opponentValue))
                {
                    _lonely /= (_factorLearnung * 1.5);
                    test = false;
                }
            }
        }
    }

    // Upper and Lower Bounds

    if(_fewerFrontierDiscs > _upperBound)
    {
        _fewerFrontierDiscs = _upperBound;
    }
    else if (_fewerFrontierDiscs < _lowerBound)
    {
        _fewerFrontierDiscs = _lowerBound;
    }

    if(_fewerMoves > _upperBound)
    {
        _fewerMoves = _upperBound;
    }
    else if (_fewerMoves < _lowerBound)
    {
        _fewerMoves = _lowerBound;
    }

    if(_moreOwnMoves > _upperBound)
    {
        _moreOwnMoves = _upperBound;
    }
    else if (_moreOwnMoves < _lowerBound)
    {
        _moreOwnMoves = _lowerBound;
    }

    if(_greedy > _upperBound)
    {
        _greedy = _upperBound;
    }
    else if (_greedy < _lowerBound)
    {
        _greedy = _lowerBound;
    }

    if(_edge > _upperBound)
    {
        _edge = _upperBound;
    }
    else if (_edge < _lowerBound)
    {
        _edge = _lowerBound;
    }

    if(_lonely > _upperBound)
    {
        _lonely = _upperBound;
    }
    else if (_lonely < _lowerBound)
    {
        _lonely = _lowerBound;
    }
}

float AdaptiveTreeAIPlayer::calculateFactor(Gameboard board, int x, int y, int opponentValue)
{
    // It is hard to find out if the opponent wants to place discs in corners. Because corners are
    // really important we will assume the opponent want to capture it.

    if((x == 0 || x == 7) && (y == 0 || y == 7))
    {
        if(board.owner(x,y) == 0 && board.play(x,y,opponentValue,true))
        {
            return _upperBound;
        }
    }

    int n = 0;
    float factor = 0;

    if(triesFewerFrontierDiscs(board,x,y,opponentValue))
    {
        factor += _fewerFrontierDiscs;
        ++n;
    }

    if(triesFewerMoves(board,x,y,opponentValue))
    {
        factor += _fewerMoves;
        ++n;
    }

    if(triesMoreOwnMoves(board,x,y,opponentValue))
    {
        factor += _moreOwnMoves;
        ++n;
    }

    if(triesGreedy(board,x,y,opponentValue))
    {
        factor += _greedy;
        ++n;
    }

    if(triesEdge(board,x,y,opponentValue))
    {
        factor += _edge;
        ++n;
    }

    if(triesLonely(board,x,y,opponentValue))
    {
        factor += _lonely;
        ++n;
    }

    if(n == 0)
    {
        return 1;
    }
    else
    {
        return factor/n;
    }
}


bool AdaptiveTreeAIPlayer::isFrontierDisc(Gameboard board, int x, int y)
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
