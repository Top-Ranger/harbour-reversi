#include "greedyaiplayer.h"
#include <QTime>

GreedyAIPlayer::GreedyAIPlayer(QObject *parent) :
    Player(parent)
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
}

void GreedyAIPlayer::doTurn()
{
    emit wantBoard();
}

bool GreedyAIPlayer::isHuman()
{
    return false;
}

void GreedyAIPlayer::getBoard(Gameboard board, int player)
{
    int max = -1;
    int old = board.points(player);
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

    if((max-old) > 6)
    {
        emit sendMessage(QString("HA! %1 points!").arg(max-old));
    }
    else
    {
        emit sendMessage(QString("OH NO! Only %1 points").arg(max-old));
    }
    emit turn(xmax, ymax);
}

void GreedyAIPlayer::humanInput(int x, int y)
{
}
