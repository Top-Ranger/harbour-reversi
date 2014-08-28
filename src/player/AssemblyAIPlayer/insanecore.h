#ifndef INSANECORE_H
#define INSANECORE_H

#include "core.h"

class InsaneCore : public Core
{
public:
    InsaneCore();
    virtual bool retirement(Gameboard board, int player);
    virtual bool mistrust(float const* const* const vote, Gameboard board, int player);
    virtual void propose(float ** const vote, Gameboard board, int player);
    virtual void correct(float ** const vote, Gameboard board, int player);
    virtual QString name() const;

private:
    int _id;
    static int _counter;
};

#endif // INSANECORE_H
