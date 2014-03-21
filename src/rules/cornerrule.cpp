#include "cornerrule.h"
#include <QDebug>

CornerRule::CornerRule(QObject *parent) :
    Rule(parent),
    _x(-1),
    _y(-1),
    _asked(false)
{
}

bool CornerRule::applicable(Gameboard board, int player)
{
    if(board.play(0,0,player,true))
    {
        _x = 0;
        _y = 0;
        _asked = true;
        return true;
    }
    else if(board.play(0,7,player,true))
    {
        _x = 0;
        _y = 7;
        _asked = true;
        return true;
    }
    else if(board.play(7,0,player,true))
    {
        _x = 7;
        _y = 0;
        _asked = true;
        return true;
    }
    else if(board.play(7,7,player,true))
    {
        _x = 7;
        _y = 7;
        _asked = true;
        return true;
    }
    else
    {
        _x = -1;
        _y = -1;
        _asked = false;
        return false;
    }
}

void CornerRule::doTurn(Gameboard board, int player)
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
        qCritical() << "FATAL ERROR: CornerRule::applicable(Gameboard board, int player) was not called";
    }
}

QString CornerRule::name()
{
    return "Corner Rule";
}
