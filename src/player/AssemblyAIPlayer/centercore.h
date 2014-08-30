#ifndef CENTERCORE_H
#define CENTERCORE_H

#include "core.h"

class CenterCore : public Core
{
public:
    CenterCore();
    virtual bool retirement(Gameboard board, int player);
    virtual bool mistrust(float const* const* const vote, Gameboard board, int player);
    virtual void propose(float ** const vote, Gameboard board, int player);
    virtual void correct(float ** const vote, Gameboard board, int player);
    virtual QString name() const;
};

#endif // CENTERCORE_H
