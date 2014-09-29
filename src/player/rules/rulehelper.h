#ifndef RULEHELPER_H
#define RULEHELPER_H

#include "../../core/gameboard.h"

namespace RuleHelper {
bool isFrontierDisc(Gameboard board, int x, int y);
bool canTakeCorner(Gameboard board, int player);
bool canGetZeroDiscs(Gameboard board, int player);
int inline opponent(int player){return player==1?2:1;}
}

#endif // RULEHELPER_H
