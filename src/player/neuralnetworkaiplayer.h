/*
  Copyright (C) 2014,2015 Marcus Soll
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

#ifndef NEURALNETWORKPLAYER_H
#define NEURALNETWORKPLAYER_H

#include <QGenericMatrix>
#include <QString>
#include "player.h"

class NeuralNetworkAIPlayer : public Player
{
public:
    NeuralNetworkAIPlayer(QObject *parent);
    virtual bool isHuman();
    virtual void doTurn(Gameboard board, int player);

public slots:
    virtual void humanInput(int x, int y);

private:
    static const char *_pathInputToHidden1;
    static const char *_pathHidden1ToHidden2;
    static const char *_pathHidden2ToOutput;
    static const float random_fachtor = 1.5;

    static const int _hiddenSize = 16;

    int _lastboard[64];

    QGenericMatrix<_hiddenSize,129,float> _inputToHidden1;
    QGenericMatrix<_hiddenSize,_hiddenSize+1,float> _hidden1ToHidden2;
    QGenericMatrix<64,_hiddenSize+1,float> _hidden2ToOutput;
};

#endif // NEURALNETWORKPLAYER_H
