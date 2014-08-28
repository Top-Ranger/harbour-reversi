#ifndef CORE_H
#define CORE_H

#include "../../core/gameboard.h"
#include <QString>

class Core
{
public:
    Core();
    virtual bool retirement(Gameboard board, int player) = 0;
    virtual bool mistrust(float const* const* const vote, Gameboard board, int player) = 0;
    virtual void propose(float ** const vote, Gameboard board, int player) = 0;
    virtual void correct(float ** const vote, Gameboard board, int player) = 0;
    virtual QString name() const = 0;
};

inline bool operator==(const Core& core1, const Core& core2) {return core1.name() == core2.name();}
#endif // CORE_H
