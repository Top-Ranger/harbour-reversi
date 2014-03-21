#ifndef MINIMISEOPPONENTMOVEMENTRULE_H
#define MINIMISEOPPONENTMOVEMENTRULE_H

#include "rule.h"

class MinimiseOpponentMovementRule : public Rule
{
public:
    explicit MinimiseOpponentMovementRule(QObject *parent = 0);
    virtual bool applicable(Gameboard board, int player);
    virtual void doTurn(Gameboard board, int player);
    virtual QString name();

private:
    bool canTakeCorner(Gameboard board, int player);
    int _x;
    int _y;
    bool _asked;
    static const int _borderMoves = 8; // 8
};

#endif // MINIMISEOPPONENTMOVEMENTRULE_H
