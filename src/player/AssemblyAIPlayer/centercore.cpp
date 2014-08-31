#include "centercore.h"
#include "assemblyaihelper.h"
#include "QDebug"

CenterCore::CenterCore()
{
}

bool CenterCore::retirement(Gameboard board, int player)
{
    int count = 0;
    for(int x = 3; x <=4; ++x)
    {
        for(int y = 3; y <= 4; ++y)
        {
            if(board.owner(x,y) == player)
            {
                ++count;
            }
        }
    }
    return count >= 3;
}

bool CenterCore::mistrust(float const* const* const vote, Gameboard board, int player)
{
    int count = 0;
    for(int x = 2; x <=5; ++x)
    {
        for(int y = 2; y <= 5; ++y)
        {
            if(board.owner(x,y) == player)
            {
                ++count;
            }
        }
    }

    return count <= 2;
}

void CenterCore::propose(float ** const vote, Gameboard board, int player)
{
    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            if(board.play(x,y,player,true))
            {
                Gameboard testboard = board;
                testboard.play(x,y,player,false);
                int count = 0;
                for(int x_center = 3; x_center <=4; ++x_center)
                {
                    for(int y_center = 3; y_center <= 4; ++y_center)
                    {
                        if(testboard.owner(x_center,y_center) == player)
                        {
                            ++count;
                        }
                    }
                }
                if(count >= 4)
                {
                    vote[x][y] = 1;
                }
                else if(count > 0)
                {
                    vote[x][y] = 1 / (5-count);
                }
            }
        }
    }
    AssemblyAI::ensureOnePossibleMove(vote,board,player);
}

void CenterCore::correct(float ** const vote, Gameboard board, int player)
{
    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            if(vote[x][y] > 0 && board.play(x,y,player,true))
            {
                Gameboard testboard = board;
                testboard.play(x,y,player,false);
                int count = 0;
                for(int x_center = 3; x_center <=4; ++x_center)
                {
                    for(int y_center = 3; y_center <= 4; ++y_center)
                    {
                        if(testboard.owner(x_center,y_center) == player)
                        {
                            ++count;
                        }
                    }
                }
                switch(count)
                {
                case 0:
                    vote[x][y] /= Core::_factorLarge;
                    break;
                case 1:
                    vote[x][y] /= Core::_factorSmall;
                    break;
                case 3:
                    vote[x][y] *= Core::_factorSmall;
                    break;
                case 4:
                    vote[x][y] *= Core::_factorLarge;
                    break;
                default:
                    break;
                }
            }
        }
    }
    AssemblyAI::ensureOnePossibleMove(vote,board,player);
}

QString CenterCore::name() const
{
    return "Center Core";
}
