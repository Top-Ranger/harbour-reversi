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
    static const int _borderRetirement = 8;
    static const int _borderMistrust = 12;
    static const int _borderSmall = 4;
    static const int _borderLarge = 6;
};

#endif // FRONTIERDISCSCORE_H
