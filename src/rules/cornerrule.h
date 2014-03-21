#ifndef CORNERRULE_H
#define CORNERRULE_H

#include "rule.h"

class CornerRule : public Rule
{
public:
    explicit CornerRule(QObject *parent = 0);
    virtual bool applicable(Gameboard board, int player);
    virtual void doTurn(Gameboard board, int player);
    virtual QString name();

private:
    int _x;
    int _y;
    bool _asked;
};

#endif // CORNERRULE_H
