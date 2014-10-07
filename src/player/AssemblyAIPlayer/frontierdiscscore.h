#ifndef FRONTIERDISCSCORE_H
#define FRONTIERDISCSCORE_H

#include "core.h"

class FrontierDiscsCore : public Core
{
public:
    FrontierDiscsCore();
    virtual bool retirement(Gameboard board, int player);
    virtual bool mistrust(float const* const* const vote, Gameboard board, int player);
    virtual void propose(float ** const vote, Gameboard board, int player);
    virtual void correct(float ** const vote, Gameboard board, int player);
    virtual QString name() const;

private:
    static const int _borderRetirement = 10;
    static const int _borderMistrust = 14;
    static const int _borderSmall = 6;
    static const int _borderLarge = 10;
};

#endif // FRONTIERDISCSCORE_H
