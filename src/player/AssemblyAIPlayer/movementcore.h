#ifndef MOVEMENTCORE_H
#define MOVEMENTCORE_H

#include "core.h"

class MovementCore : public Core
{
public:
public:
    MovementCore();
    virtual bool retirement(Gameboard board, int player);
    virtual bool mistrust(float const* const* const vote, Gameboard board, int player);
    virtual void propose(float ** const vote, Gameboard board, int player);
    virtual void correct(float ** const vote, Gameboard board, int player);
    virtual QString name() const;
};

#endif // MOVEMENTCORE_H
