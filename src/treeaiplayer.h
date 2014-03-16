#ifndef TREEAIPLAYER_H
#define TREEAIPLAYER_H

#include "player.h"

class TreeAIPlayer : public Player
{
public:
    explicit TreeAIPlayer(QObject *parent = 0);
    virtual void doTurn();
    virtual bool isHuman();
    virtual void getBoard(Gameboard board, int player);

public slots:
    virtual void humanInput(int x, int y);

private:
    float buildTree(Gameboard board, int player, int I, int depth);
    int inline opponent(int player);
    float calculateScore(Gameboard board, int I);
    static const int _depth = 4;
    static const int _composure = 5;
    static const int _valueCorner = 100;
    static const float _factorStone = 0.5;
};

#endif // TREEAIPLAYER_H
