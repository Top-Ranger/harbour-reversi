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

#include "cornerrule.h"
#include <QDebug>

CornerRule::CornerRule(QObject *parent) :
    Rule(parent),
    _x(-1),
    _y(-1),
    _asked(false)
{
}

bool CornerRule::applicable(Gameboard board, int player)
{
    if(board.play(0,0,player,true))
    {
        _x = 0;
        _y = 0;
        _asked = true;
        return true;
    }
    else if(board.play(0,7,player,true))
    {
        _x = 0;
        _y = 7;
        _asked = true;
        return true;
    }
    else if(board.play(7,0,player,true))
    {
        _x = 7;
        _y = 0;
        _asked = true;
        return true;
    }
    else if(board.play(7,7,player,true))
    {
        _x = 7;
        _y = 7;
        _asked = true;
        return true;
    }
    else
    {
        _x = -1;
        _y = -1;
        _asked = false;
        return false;
    }
}

void CornerRule::doTurn(Gameboard board, int player)
{
    if(_asked)
    {
        _asked = false;
        emit turn(_x,_y);
        _x = -1;
        _y = -1;
    }
    else
    {
        qCritical() << "FATAL ERROR in " __FILE__ << " " << __LINE__ << ": CornerRule::applicable(Gameboard board, int player) was not called";
    }
}

QString CornerRule::name()
{
    return tr("Corner Rule");
}
