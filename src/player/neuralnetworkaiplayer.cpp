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

#include <QGenericMatrix>
#include <QDebug>
#include <limits>
#include <QFile>
#include <QTextStream>
#include "neuralnetworkaiplayer.h"

const char *NeuralNetworkAIPlayer::_pathInputToHidden = ":NeuralNetworkAIPlayer/inputToHidden.txt";
const char *NeuralNetworkAIPlayer::_pathHiddenToOutput = ":NeuralNetworkAIPlayer/hiddenToOutput.txt";

NeuralNetworkAIPlayer::NeuralNetworkAIPlayer(QObject *parent) :
    Player(parent),
    _inputToHidden(),
    _hiddenToOutput()
{
    for(int i = 0; i < 64; ++i)
    {
        _lastboard[i] = 0;
    }

    // Initialize _inputToHidden
    QFile ith(_pathInputToHidden);
    ith.open(QIODevice::ReadOnly);
    QTextStream ithStream(&ith);
    for(int i = 0; i < _hiddenSize*128; ++i)
    {
        ithStream >> _inputToHidden(i/_hiddenSize, i%_hiddenSize);
    }
    ith.close();

    // Initialize _hiddenToOutput
    QFile hto(_pathHiddenToOutput);
    hto.open(QIODevice::ReadOnly);
    QTextStream htoStream(&hto);
    for(int i = 0; i < _hiddenSize*64; ++i)
    {
        htoStream >> _hiddenToOutput(i/64, i%64);
    }
    hto.close();
}

bool NeuralNetworkAIPlayer::isHuman()
{
    return false;
}

void NeuralNetworkAIPlayer::doTurn(Gameboard board, int player)
{
    QGenericMatrix<128,1,float> input;

    for(int i = 0; i < 64; ++i)
    {
        float value = -1.0;
        if(board.owner(i%8, i/8) == player)
        {
            value = 1.0;
        }
        else if(board.owner(i%8, i/8) == 0)
        {
            value = 0.0;
        }
        input(0,i) = value;
    }

    for(int i = 64; i < 128; ++i)
    {
        input(0,i) = _lastboard[i-64];
        _lastboard[i-64] = input(0,i-64);
    }

    QGenericMatrix<64,1,float> output = (input * _inputToHidden) * _hiddenToOutput;

    float max = -1000000000;
    int turn_save = -1;
    for(int i = 0; i < 64; ++i)
    {
        if(output(0,i) > max && board.play(i%8, i/8,player,true))
        {
            max = output(0,i);
            turn_save = i;
        }
    }
    if(turn_save != -1)
    {
        qWarning() << max;
        emit turn(turn_save%8, turn_save/8);
    }
    else
    {
        qCritical() << "CRITICAL ERROR in " __FILE__ << " " << __LINE__ << ": Neural Network AI has no play";
    }
}

void NeuralNetworkAIPlayer::humanInput(int x, int y)
{
}
