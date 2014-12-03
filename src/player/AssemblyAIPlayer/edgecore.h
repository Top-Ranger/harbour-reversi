#ifndef EDGECORE_H
#define EDGECORE_H

#include "core.h"

class EdgeCore : public Core
{
public:
    EdgeCore();
    virtual bool retirement(Gameboard board, int player);
    virtual int mistrust(float const* const* const vote, Gameboard board, int player);
    virtual void propose(float ** const vote, Gameboard board, int player);
    virtual void correct(float ** const vote, Gameboard board, int player);
    virtual QString name() const;

private:
    static const int _boarderTokens = 5;
};

#endif // EDGECORE_H
