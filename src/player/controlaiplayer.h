#ifndef CONTROLAIPLAYER_H
#define CONTROLAIPLAYER_H

#include "player.h"
#include <QList>

typedef void(*function)(Gameboard, int);

class ControlAIPlayer : public Player
{
    Q_OBJECT
public:
    explicit ControlAIPlayer(QObject *parent = 0);
    virtual void doTurn();
    virtual bool isHuman();
    virtual void getBoard(Gameboard board, int player);

    static void dummy(Gameboard board, int player);
    static void dummy2(Gameboard board, int player);

public slots:
    virtual void humanInput(int x, int y);

private:
    static const int _sizeAvailableFunctions = 2;
    static const int _sizeCurrentFunctions = 3;

    void (*_availableFunctions[_sizeAvailableFunctions])(Gameboard, int);
    void (* _currentFunctions[_sizeCurrentFunctions])(Gameboard, int);
    int _priority[8][8];
};

#endif // CONTROLAIPLAYER_H
