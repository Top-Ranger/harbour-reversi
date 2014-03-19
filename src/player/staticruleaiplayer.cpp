#include "staticruleaiplayer.h"
#include "../rules/greedyrule.h"
#include <QDebug>

StaticRuleAIPlayer::StaticRuleAIPlayer(QObject *parent) :
    Player(parent)
{
    _rulelist.append(new GreedyRule(this));

    QList<Rule *> ::const_iterator iterator;
    for(iterator=_rulelist.begin(); iterator != _rulelist.end();++iterator)
    {
        QObject::connect((*iterator),SIGNAL(turn(int,int)),this,SLOT(getTurn(int,int)));
    }
}

StaticRuleAIPlayer::~StaticRuleAIPlayer()
{
    QList<Rule *> ::const_iterator iterator;
    for(iterator=_rulelist.begin(); iterator != _rulelist.end();++iterator)
    {
        delete (*iterator);
    }
}

void StaticRuleAIPlayer::doTurn()
{
    emit wantBoard();
}

bool StaticRuleAIPlayer::isHuman()
{
    return false;
}

void StaticRuleAIPlayer::getBoard(Gameboard board, int player)
{
    QList<Rule *> ::const_iterator iterator;
    for(iterator=_rulelist.begin(); iterator != _rulelist.end();++iterator)
    {
        if((*iterator)->applicable(board,player))
        {
            emit sendMessage(QString("Now using rule \"%1\"").arg((*iterator)->name()));
            (*iterator)->doTurn(board,player);
            qDebug() << "11344";
            return;
        }
    }

    // If everything fails do a greedy rule
    qWarning() << "ERROR: Did not find any rule, using default \"Greedy Rule\"";
    GreedyRule rule;
    if(!rule.applicable(board,player))
    {
        emit sendMessage("Static Rule AI hasn't any rules to apply...");
        qCritical() << "FATAL ERROR: Static Rule AI has no rule to apply";
        return;
    }
    QObject::connect(&rule,SIGNAL(turn(int,int)),this,SLOT(getTurn(int,int)));
    emit sendMessage("Using fallback \"Greedy Rule\"");
    rule.doTurn(board,player);
    QObject::disconnect(&rule,SIGNAL(turn(int,int)),this,SLOT(getTurn(int,int)));
}

void StaticRuleAIPlayer::humanInput(int x, int y)
{
}

void StaticRuleAIPlayer::getTurn(int x, int y)
{
    emit turn(x,y);
}
