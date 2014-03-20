#ifndef RULE_H
#define RULE_H

#include <QObject>
#include "../core/gameboard.h"

class Rule : public QObject
{
    Q_OBJECT
public:
    explicit Rule(QObject *parent = 0);
    virtual bool applicable(Gameboard board, int player) = 0;
    virtual void doTurn(Gameboard board, int player) = 0;
    virtual QString name() = 0;

signals:
    void turn(int x, int y);

public slots:

protected:
    virtual int opponent(int player);
};

#endif // RULE_H
