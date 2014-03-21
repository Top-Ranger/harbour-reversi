#ifndef MAXIMISEOWNMOVEMENTRULE_H
#define MAXIMISEOWNMOVEMENTRULE_H

#include "rule.h"

class MaximiseOwnMovementRule : public Rule
{
public:
    explicit MaximiseOwnMovementRule(QObject *parent = 0);
    virtual bool applicable(Gameboard board, int player);
    virtual void doTurn(Gameboard board, int player);
    virtual QString name();

private:
    int _x;
    int _y;
    bool _asked;
    static const int _borderMoves = 7;
};

#endif // MAXIMISEOWNMOVEMENTRULE_H
