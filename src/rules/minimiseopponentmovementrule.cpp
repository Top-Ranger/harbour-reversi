#include "minimiseopponentmovementrule.h"
#include <QDebug>

const int MinimiseOpponentMovementRule::_borderMoves;

MinimiseOpponentMovementRule::MinimiseOpponentMovementRule(QObject *parent) :
    Rule(parent),
    _x(-1),
    _y(-1),
    _asked(false)
{
}

bool MinimiseOpponentMovementRule::applicable(Gameboard board, int player)
{
    int min = 100;
    int x = qrand()%8;
    int y = qrand()%8;
    int xstart = x;
    int ystart = y;
    int xmin = -1;
    int ymin = -1;
    int opponentPlayer = opponent(player);
    Gameboard testboard;

    do
    {
        do
        {
            testboard = board;

            if(testboard.play(x,y,player))
            {
                int temp = 0;

                for(int opponentX = 0; opponentX < 8; ++opponentX)
                {
                    for(int opponentY = 0; opponentY < 8; ++opponentY)
                    {
                        if(testboard.play(opponentX, opponentY,opponentPlayer,true))
                        {
                            ++temp;
                        }
                    }
                }

                if(temp < min)
                {
                    min = temp;
                    xmin = x;
                    ymin = y;
                }
            }

            y = (y+1)%8;
        }while(y != ystart);

        x = (x+1)%8;
    }while(x != xstart);

    if(min <= _borderMoves)
    {
        _x = xmin;
        _y = ymin;
        _asked = true;
    }
    else
    {
        _x = -1;
        _y = -1;
        _asked = false;
    }
    return _asked;
}

void MinimiseOpponentMovementRule::doTurn(Gameboard board, int player)
{
    if(_asked)
    {
        _asked = false;
        emit turn(_x,_y);
        _x = -1;
        _y = -1;
    }
    else
    {
        qCritical() << "FATAL ERROR: MinimiseOpponentMovementRule::applicable(Gameboard board, int player) was not called";
    }
}

QString MinimiseOpponentMovementRule::name()
{
    return "Minimise Opponent Movement Rule";
}
