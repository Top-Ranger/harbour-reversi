#ifndef GREEDYRULE_H
#define GREEDYRULE_H

#include "rule.h"

class GreedyRule : public Rule
{
public:
    explicit GreedyRule(QObject *parent = 0);
    virtual bool applicable(Gameboard board, int player);
    virtual void doTurn(Gameboard board, int player);
    virtual QString name();
};

#endif // GREEDYRULE_H
