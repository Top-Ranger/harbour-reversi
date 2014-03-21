#include "maximiseownmovementrule.h"
#include <QDebug>
#include <QTime>

const int MaximiseOwnMovementRule::_borderMoves;

MaximiseOwnMovementRule::MaximiseOwnMovementRule(QObject *parent) :
    Rule(parent),
    _x(-1),
    _y(-1),
    _asked(false)
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
}

bool MaximiseOwnMovementRule::applicable(Gameboard board, int player)
{
    int max = -1;
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
                int temp = 0;

                for(int newX = 0; newX < 8; ++newX)
                {
                    for(int newY = 0; newY < 8; ++newY)
                    {
                        if(testboard.play(newX, newY,player,true))
                        {
                            ++temp;
                        }
                    }
                }

                if(temp > max && (!canTakeCorner(testboard, opponent(player))))
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

    if(max >= _borderMoves)
    {
        _x = xmax;
        _y = ymax;
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

void MaximiseOwnMovementRule::doTurn(Gameboard board, int player)
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
        qCritical() << "FATAL ERROR: MaximiseOwnMovementRule::applicable(Gameboard board, int player) was not called";
    }
}

QString MaximiseOwnMovementRule::name()
{
    return "Maximise Own Movement Rule";
}

bool MaximiseOwnMovementRule::canTakeCorner(Gameboard board, int player)
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
