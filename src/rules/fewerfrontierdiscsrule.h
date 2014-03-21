#ifndef FEWERFRONTIERDISCSRULE_H
#define FEWERFRONTIERDISCSRULE_H

#include "rule.h"

class FewerFrontierDiscsRule : public Rule
{
public:
    explicit FewerFrontierDiscsRule(QObject *parent = 0);
    virtual bool applicable(Gameboard board, int player);
    virtual void doTurn(Gameboard board, int player);
    virtual QString name();

private:
    bool isFrontierDisc(Gameboard board, int x, int y);
};

#endif // FEWERFRONTIERDISCSRULE_H
