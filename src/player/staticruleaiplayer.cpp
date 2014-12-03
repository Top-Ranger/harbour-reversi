/*
  Copyright (C) 2014 Marcus Soll
  All rights reserved.

  You may use this file under the terms of BSD license as follows:

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Jolla Ltd nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "staticruleaiplayer.h"
#include "../player/rules/greedyrule.h"
#include "../player/rules/openingrule.h"
#include "../player/rules/endgamerule.h"
#include "../player/rules/cornerrule.h"
#include "../player/rules/minimiseopponentmovementrule.h"
#include "../player/rules/maximiseownmovementrule.h"
#include "../player/rules/fewerfrontierdiscsrule.h"
#include <QDebug>

StaticRuleAIPlayer::StaticRuleAIPlayer(QObject *parent) :
    Player(parent)
{
    _rulelist.append(new CornerRule(this));
    _rulelist.append(new OpeningRule(this));
    _rulelist.append(new EndgameRule(this));
    _rulelist.append(new MinimiseOpponentMovementRule(this));
    _rulelist.append(new MaximiseOwnMovementRule(this));
    _rulelist.append(new FewerFrontierDiscsRule(this));

    QList<Rule *> ::const_iterator iterator;
    for(iterator=_rulelist.begin(); iterator != _rulelist.end();++iterator)
    {
        QObject::connect((*iterator),SIGNAL(turn(int,int)),this,SLOT(getTurn(int,int)));
    }
}

StaticRuleAIPlayer::~StaticRuleAIPlayer()
{
    QList<Rule *> ::const_iterator iterator;
    for(iterator=_rulelist.begin(); iterator != _rulelist.end();++iterator)
    {
        delete (*iterator);
    }
}

bool StaticRuleAIPlayer::isHuman()
{
    return false;
}

void StaticRuleAIPlayer::doTurn(Gameboard board, int player)
{
    QList<Rule *> ::const_iterator iterator;
    for(iterator=_rulelist.begin(); iterator != _rulelist.end();++iterator)
    {
        if((*iterator)->applicable(board,player))
        {
            emit sendMessage(QString(tr("Now using rule \"%1\"")).arg((*iterator)->name()));
            (*iterator)->doTurn(board,player);
            return;
        }
    }

    // If everything fails do a greedy rule
    qWarning() << "ERROR: Did not find any rule, using default \"Greedy Rule\"";
    GreedyRule rule;
    if(!rule.applicable(board,player))
    {
        emit sendMessage(tr("Static Rule AI hasn't any rules to apply..."));
        qCritical() << "CRITICAL ERROR in " __FILE__ << " " << __LINE__ << ": Static Rule AI has no rule to apply";
        return;
    }
    QObject::connect(&rule,SIGNAL(turn(int,int)),this,SLOT(getTurn(int,int)));
    emit sendMessage(tr("Using fallback \"Greedy Rule\""));
    rule.doTurn(board,player);
    QObject::disconnect(&rule,SIGNAL(turn(int,int)),this,SLOT(getTurn(int,int)));
}

void StaticRuleAIPlayer::humanInput(int x, int y)
{
}

void StaticRuleAIPlayer::getTurn(int x, int y)
{
    emit turn(x,y);
}
