/*
  Copyright (C) 2014,2016 Marcus Soll
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

#ifndef ASSEMBLYAIPLAYER_H
#define ASSEMBLYAIPLAYER_H

#include "player.h"

#include "AssemblyAIPlayer/core.h"

#include <QList>

class AssemblyAIPlayer : public Player
{
    Q_OBJECT
public:
    explicit AssemblyAIPlayer(QObject *parent = 0);
    ~AssemblyAIPlayer();
    virtual bool isHuman();
    virtual void doTurn(Gameboard board, int player);

public slots:
    virtual void humanInput(int x, int y);

private:
    QList<Core *> _inactiveCores;
    Core * _activeCore;

    // A core can propose a value between 0-1
    // A core can correct a value by Core::_factorSmall or Core::_factorLarge
    float **_vote;

    static const int _maxChanges = 10;
    static const int _neededToChange = 4;
};

#endif // ASSEMBLYAIPLAYER_H
