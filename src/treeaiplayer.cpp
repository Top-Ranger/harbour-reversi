#include "treeaiplayer.h"
#include <QTime>

const int TreeAIPlayer::_depth;
const int TreeAIPlayer::_composure;
const int TreeAIPlayer::_valueCorner;
const float TreeAIPlayer::_factorStone;

TreeAIPlayer::TreeAIPlayer(QObject *parent) :
    Player(parent)
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
}

void TreeAIPlayer::doTurn()
{
    emit wantBoard();
}

bool TreeAIPlayer::isHuman()
{
    return false;
}

void TreeAIPlayer::getBoard(Gameboard board, int player)
{
    float max = -1000000;
    int x = qrand()%8;
    int y = qrand()%8;
    int xstart = x;
    int ystart = y;
    int xmax = -1;
    int ymax = -1;
    Gameboard testboard;
    int composure = qrand() % _composure;

    do
    {
        do
        {
            testboard = board;

            if(testboard.play(x,y,player))
            {
                if(composure == 0)
                {
                    if(testboard.points(player) > max)
                    {
                        max = testboard.points(player);
                        xmax = x;
                        ymax = y;
                    }
                }
                else
                {
                    float temp = buildTree(board, opponent(player), player, _depth);
                    if(temp > max)
                    {
                        max = temp;
                        xmax = x;
                        ymax = y;
                    }
                }
            }

            y = (y+1)%8;
        }while(y != ystart);

        x = (x+1)%8;
    }while(x != xstart);

    if(composure == 0)
    {
        emit sendMessage(QString("I want %1:%2! NOW!").arg(xmax).arg(ymax));
    }
    else
    {
        emit sendMessage(QString("I'll do %1:%2, that should be the best turn with factor %3").arg(xmax).arg(ymax).arg(max));
    }
    emit turn(xmax,ymax);
}

void TreeAIPlayer::humanInput(int x, int y)
{
}

float TreeAIPlayer::buildTree(Gameboard board, int player, int I, int depth)
{
    --depth;
    int n = 0;
    float advantage = 0;

    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            if(board.play(x,y,player,true))
            {
                if(depth == 0)
                {
                    board.play(x,y,player,false);
                    return calculateScore(board, I);
                }
                else
                {
                    Gameboard newBoard = board;
                    newBoard.play(x,y,player,false);
                    advantage += buildTree(newBoard,opponent(player),I,depth);
                    ++n;
                }
            }
        }
    }
    if(n == 0)
    {
        return 0;
    }
    else
    {
        return advantage / n;
    }
}

int inline TreeAIPlayer::opponent(int player)
{
    return player==1?2:1;
}

float TreeAIPlayer::calculateScore(Gameboard board, int I)
{
    float score = 0;

    if(board.owner(0,0) == I)
    {
        score += _valueCorner;
    }
    else if(board.owner(0,0) == opponent(I))
    {
        score -= _valueCorner;
    }
    if(board.owner(7,0) == I)
    {
        score += _valueCorner;
    }
    else if(board.owner(7,0) == opponent(I))
    {
        score -= _valueCorner;
    }
    if(board.owner(0,7) == I)
    {
        score += _valueCorner;
    }
    else if(board.owner(0,7) == opponent(I))
    {
        score -= _valueCorner;
    }
    if(board.owner(7,7) == I)
    {
        score += _valueCorner;
    }
    else if(board.owner(7,7) == opponent(I))
    {
        score -= _valueCorner;
    }

    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; y++)
        {
            int pointsX = (x)>(7-x)?x:7-x;
            int pointsY = (y)>(7-y)?y:7-y;

            if(board.owner(x,y) == I)
            {
                score += _factorStone * (pointsX > pointsY ? pointsX : pointsY);
            }
            else if(board.owner(x,y) == opponent(I))
            {
                score -= _factorStone * (pointsX > pointsY ? pointsX : pointsY);
            }
        }
    }
    return score;
}
