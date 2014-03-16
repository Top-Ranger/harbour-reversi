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
    float buildTree(Gameboard board, int player, int I, int old, int opponentOld, int depth);
    int inline opponent(int player);
    float calculateScore(Gameboard board, int I, int change, int opponentChange);
    static const int _depth = 4; // 4
    static const int _composure = 5; // 5
    static const int _valueCorner = 100; // 100
    static const float _factorStone = 0.37; // 0.37
    static const int _valueFullBoard = 500; // 500
    static const float _factorRatio = 40; // 40
    static const float _factorChange = 15.5; // 15.5
    static const float _factorOpponentChange = 25.5; // 25.5
};

#endif // TREEAIPLAYER_H
