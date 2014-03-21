#ifndef OPENINGRULE_H
#define OPENINGRULE_H

#include "rule.h"

class OpeningRule : public Rule
{
public:
    explicit OpeningRule(QObject *parent = 0);
    virtual bool applicable(Gameboard board, int player);
    virtual void doTurn(Gameboard board, int player);
    virtual QString name();

private:
    int calculateScore(Gameboard board, int player);
    bool isFrontierDisc(Gameboard board, int x, int y);

    static const int _borderDiscs = 16;
    static const int _valueCenter = 5;
    static const int _factorDistribution = 5;
    static const int _valueFrontierDisc = 10;
};

#endif // OPENINGRULE_H
