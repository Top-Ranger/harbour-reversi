#include "greedyrule.h"

GreedyRule::GreedyRule(QObject *parent) :
    Rule(parent)
{
}

bool GreedyRule::applicable(Gameboard board, int player)
{
    return board.isTurnPossible(player);
}

void GreedyRule::doTurn(Gameboard board, int player)
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
                if(testboard.points(player) > max)
                {
                    max = testboard.points(player);
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

QString GreedyRule::name()
{
    return "Greedy Rule";
}
