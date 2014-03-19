#ifndef STATICRULEAIPLAYER_H
#define STATICRULEAIPLAYER_H

#include <QList>
#include "player.h"
#include "../rules/rule.h"

class StaticRuleAIPlayer : public Player
{
    Q_OBJECT
public:
    explicit StaticRuleAIPlayer(QObject *parent = 0);
    ~StaticRuleAIPlayer();
    virtual void doTurn();
    virtual bool isHuman();
    virtual void getBoard(Gameboard board, int player);

public slots:
    virtual void humanInput(int x, int y);
    virtual void getTurn(int x, int y);

private:
    QList<Rule *> _rulelist;
};

#endif // STATICRULEAIPLAYER_H
