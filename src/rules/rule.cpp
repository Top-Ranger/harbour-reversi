#include "rule.h"

Rule::Rule(QObject *parent) :
    QObject(parent)
{
}

int Rule::opponent(int player)
{
    return player==1?2:1;
}
