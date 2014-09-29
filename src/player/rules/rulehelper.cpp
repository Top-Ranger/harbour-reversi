#include "rulehelper.h"

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
