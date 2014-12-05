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

#ifndef CORE_H
#define CORE_H

#include "../../core/gameboard.h"
#include <QString>

class Core
{
public:
    Core();
    virtual ~Core();
    virtual bool retirement(Gameboard board, int player) = 0;
    virtual int mistrust(float const* const* const vote, Gameboard board, int player) = 0;
    virtual void propose(float ** const vote, Gameboard board, int player) = 0;
    virtual void correct(float ** const vote, Gameboard board, int player) = 0;
    virtual QString name() const = 0;

protected:
    static const float _factorSmall = 1.1;
    static const float _factorLarge = 1.2;

    static const int _mistrustSmall = 1;
    static const int _mistrustLarge = 3;
    static const int _mistrustEmergency = 100;
    static const int _noMistrust = 0;
};

// Two cores are the same if they have the same name. As there should only be one core of every type in AssemblyAIPlayer, this is a easy and cheap way of comparing cores.
inline bool operator==(const Core& core1, const Core& core2) {return core1.name() == core2.name();}
#endif // CORE_H
