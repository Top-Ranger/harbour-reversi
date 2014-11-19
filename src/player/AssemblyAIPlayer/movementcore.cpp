#include "movementcore.h"
#include "assemblyaihelper.h"

namespace {
int countPossiblePlays(Gameboard board, int player, int x, int y)
{
    Gameboard testboard = board;
    if(testboard.play(x,y,player))
    {
        int count = 0;
        for(int xi = 0; xi < 8; ++xi)
        {
            for(int yi = 0; yi < 8; ++yi)
            {
                if(testboard.play(xi,yi,player,true))
                {
                    ++count;
                }
            }
        }
        return count;
    }
    else
    {
        return -1;
    }
}
}

MovementCore::MovementCore()
{
}

bool MovementCore::retirement(Gameboard board, int player)
{
    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            if(board.play(x,y,player,true) && countPossiblePlays(board,player,x,y) < 5)
            {
                return false;
            }
        }
    }
    return true;
}

bool MovementCore::mistrust(float const* const* const vote, Gameboard board, int player)
{
    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            if(vote[x][y] > 0 && board.play(x,y,player,true))
            {
                int count = countPossiblePlays(board,player,x,y);
                if(count == -1 || count == 0)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

void MovementCore::propose(float ** const vote, Gameboard board, int player)
{
    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            if(board.play(x,y,player,true))
            {
                int count = countPossiblePlays(board,player,x,y);
                if(count != -1)
                {
                    vote[x][y] = 1 / (65 - count);
                }
            }
        }
    }
    AssemblyAI::ensureOnePossibleMove(vote,board,player);
}

void MovementCore::correct(float ** const vote, Gameboard board, int player)
{
    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            if(vote[x][y] > 0 && board.play(x,y,player,true))
            {
                switch(countPossiblePlays(board,player,x,y))
                {
                case -1:
                case 0:
                case 1:
                case 2:
                    vote[x][y] /= Core::_factorLarge;
                    break;
                case 3:
                case 4:
                    vote[x][y] /= Core::_factorSmall;
                    break;
                case 5:
                case 6:
                case 7:
                    break;
                case 8:
                case 9:
                    vote[x][y] *= Core::_factorSmall;
                    break;
                default:
                    vote[x][y] *= Core::_factorLarge;
                }
            }
        }
    }
}

QString MovementCore::name() const
{
    return "Movement Core";
}
