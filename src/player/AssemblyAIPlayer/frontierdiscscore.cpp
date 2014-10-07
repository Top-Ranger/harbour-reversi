#include "frontierdiscscore.h"

namespace {

bool isFrontierDisc(Gameboard board, int x, int y)
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

int countFrontierDiscs(Gameboard board, int player)
{
    int count = 0;

    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            if(board.owner(x,y) == player && isFrontierDisc(board,x,y))
            {
                ++count;
            }
        }
    }

    return count;
}
}

FrontierDiscsCore::FrontierDiscsCore()
{
}

bool FrontierDiscsCore::retirement(Gameboard board, int player)
{
    return countFrontierDiscs(board,player) <= _borderRetirement;
}

bool FrontierDiscsCore::mistrust(float const* const* const vote, Gameboard board, int player)
{
    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            if(vote[x][y] > 0 && board.play(x,y,player,true))
            {
                Gameboard testboard = board;
                testboard.play(x,y,player,false);
                if(countFrontierDiscs(testboard, player) >= _borderMistrust)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

void FrontierDiscsCore::propose(float ** const vote, Gameboard board, int player)
{
    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            if(board.play(x,y,player,true))
            {
                Gameboard testboard = board;
                testboard.play(x,y,player,false);
                vote[x][y] = 1 / (65 - countFrontierDiscs(board,player));
            }
        }
    }
}

void FrontierDiscsCore::correct(float ** const vote, Gameboard board, int player)
{
    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            if(vote[x][y] > 0 && board.play(x,y,player,true))
            {
                Gameboard testboard = board;
                testboard.play(x,y,player,false);
                if(countFrontierDiscs(testboard, player) >= _borderLarge)
                {
                    vote[x][y] /= Core::_factorLarge;
                }
                else if(countFrontierDiscs(testboard, player))
                {
                    vote[x][y] /= Core::_factorSmall;
                }
            }
        }
    }
}

QString FrontierDiscsCore::name() const
{
    return "Frontier Discs Core";
}
