#include "insanecore.h"
#include <QTime>

int InsaneCore::_counter = 0;

InsaneCore::InsaneCore() :
    Core(),
    _id(++_counter)
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
}

bool InsaneCore::retirement(Gameboard board, int player)
{
    return (qrand()%2 == 0);
}

int InsaneCore::mistrust(float const* const* const vote, Gameboard board, int player)
{
    return (qrand()%60 == 0)?Core::_factorSmall:Core::_noMistrust;
}

void InsaneCore::propose(float ** const vote, Gameboard board, int player)
{
    int count = 0;
    do{
        for(int i = 0; i < 10 && count < 3; ++i)
        {
            int x = qrand()%8;
            int y = qrand()%8;
            if(board.play(x,y,player,true))
            {
                vote[x][y] = 1;
                ++count;
            }
        }
    }while(count == 0);
}

void InsaneCore::correct(float ** const vote, Gameboard board, int player)
{
    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            if(vote[x][y] != 0)
            {
                switch(qrand()%8)
                {
                case 0:
                    vote[x][y] *= Core::_factorSmall;
                    break;
                case 1:
                    vote[x][y] *= Core::_factorLarge;
                    break;
                case 2:
                    vote[x][y] /= Core::_factorSmall;
                    break;
                case 3:
                    vote[x][y] /= Core::_factorLarge;
                    break;
                default:
                    break;
                }
            }
        }
    }
}

QString InsaneCore::name() const
{
    return QString("InsaneCore Nr. %1").arg(_id);
}
