#include "fewerfrontierdiscsrule.h"
#include <QTime>

FewerFrontierDiscsRule::FewerFrontierDiscsRule(QObject *parent) :
    Rule(parent)
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
}

bool FewerFrontierDiscsRule::applicable(Gameboard board, int player)
{
    return board.isTurnPossible(player);
}

void FewerFrontierDiscsRule::doTurn(Gameboard board, int player)
{
    int max = 100;
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
            int temp = 0;

            if(testboard.play(x,y,player))
            {
                for(int newX = 0; newX < 8; ++newX)
                {
                    for(int newY = 0; newY < 8; ++newY)
                    {
                        if(isFrontierDisc(testboard,x,y))
                        {
                            ++temp;
                        }
                    }
                }

                if(temp < max)
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

QString FewerFrontierDiscsRule::name()
{
    return "Fewer Frontier Discs Rule";
}

bool FewerFrontierDiscsRule::isFrontierDisc(Gameboard board, int x, int y)
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
