#include "openingrule.h"
#include <limits>
#include <QTime>

const int OpeningRule::_borderDiscs;
const int OpeningRule::_valueCenter;
const int OpeningRule::_factorDistribution;
const int OpeningRule::_valueFrontierDisc;

OpeningRule::OpeningRule(QObject *parent) :
    Rule(parent)
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
}


bool OpeningRule::applicable(Gameboard board, int player)
{
    return (board.points(player) + board.points(opponent(player))) <= _borderDiscs;
}

void OpeningRule::doTurn(Gameboard board, int player)
{
    int max = std::numeric_limits<int>::min();
    int x = qrand()%8;
    int y = qrand()%8;
    int xstart = x;
    int ystart = y;
    int xmax = -1;
    int ymax = -1;
    Gameboard testboard;

    do
    {
        do
        {
            testboard = board;

            if(testboard.play(x,y,player))
            {
                int temp = calculateScore(testboard, player);
                if(temp > max)
                {
                    max = temp;
                    xmax = x;
                    ymax = y;
                }
            }

            y = (y+1)%8;
        }while(y != ystart);

        x = (x+1)%8;
    }while(x != xstart);

    emit turn(xmax, ymax);
}

QString OpeningRule::name()
{
    return "Opening Rule";
}

int OpeningRule::calculateScore(Gameboard board, int player)
{
    int score = (((float) board.points(opponent(player)))/board.points(player)) * _factorDistribution;

    for(int x = 3; x <=4; ++x)
    {
        for(int y = 3; y <=4; ++y)
        {
            if(board.owner(x,y) == player)
            {
                score += _valueCenter;
            }
            else if (board.owner(x,y) == opponent(player))
            {
                score -= _valueCenter;
            }
        }
    }

    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            if(isFrontierDisc(board,x,y))
            {
                if(board.owner(x,y) == player)
                {
                    score -= _valueFrontierDisc;
                }
                else if (board.owner(x,y) == opponent(player))
                {
                    score += _valueCenter;
                }
            }
        }
    }

    return score;
}

bool OpeningRule::isFrontierDisc(Gameboard board, int x, int y)
{
    for(int deltax=-1; deltax<=1; ++deltax)
    {
        for(int deltay=-1; deltay<=1; ++deltay)
        {
            if(deltax != 0 || deltay != 0)
            {
                if((x-deltax >= 0) && (y-deltay >= 0) && (x+deltax <= 7) && (y+deltay <= 7))
                {
                    if(board.owner(x,y) == 0)
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
