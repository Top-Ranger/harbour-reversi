#ifndef GREEDYAIPLAYER_H
#define GREEDYAIPLAYER_H
#include "gameboard.h"
#include "player.h"

class GreedyAIPlayer : public Player
{
public:
    explicit GreedyAIPlayer(QObject *parent = 0);
    virtual void doTurn();
    virtual bool isHuman();
    virtual void getBoard(Gameboard board, int player);

public slots:
    virtual void humanInput(int x, int y);
};

#endif // GREEDYAIPLAYER_H
