#include "endgamerule.h"
#include "greedyrule.h"

const int EndgameRule::_borderDiscs;

EndgameRule::EndgameRule(QObject *parent) :
    Rule(parent)
{
}

bool EndgameRule::applicable(Gameboard board, int player)
{
    return (board.points(player) + board.points(opponent(player))) >= _borderDiscs;
}

void EndgameRule::doTurn(Gameboard board, int player)
{
    // Find plays where the opponent can't do anything after that
    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            Gameboard temp = board;
            if(temp.play(x,y,player) && temp.isTurnPossible(opponent(player)))
            {
                emit turn(x,y);
                return;
            }
        }
    }

    //TODO: BETTER RULES

    //If everything fails do a greedy rule
    GreedyRule fallback;
    QObject::connect(&fallback,SIGNAL(turn(int,int)),this,SLOT(getTurn(int,int)));
    fallback.doTurn(board,player);
    QObject::disconnect(&fallback,SIGNAL(turn(int,int)),this,SLOT(getTurn(int,int)));

}

QString EndgameRule::name()
{
    return "Endgame Rule";
}

void EndgameRule::getTurn(int x, int y)
{
    emit turn(x,y);
}
