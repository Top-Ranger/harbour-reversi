#ifndef ENDGAMERULE_H
#define ENDGAMERULE_H

#include "rule.h"

class EndgameRule : public Rule
{
    Q_OBJECT
public:
    explicit EndgameRule(QObject *parent = 0);
    virtual bool applicable(Gameboard board, int player);
    virtual void doTurn(Gameboard board, int player);
    virtual QString name();

private slots:
    void getTurn(int x, int y);

private:
    static const int _borderDiscs = 50;
};

#endif // ENDGAMERULE_H
