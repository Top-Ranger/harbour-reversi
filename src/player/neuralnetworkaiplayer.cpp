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
#include <QTime>
#include <cmath>
#include "neuralnetworkaiplayer.h"

const char *NeuralNetworkAIPlayer::_pathInputToHidden1 = ":NeuralNetworkAIPlayer/inputToHidden1.txt";
const char *NeuralNetworkAIPlayer::_pathHidden1ToHidden2 = ":NeuralNetworkAIPlayer/hidden1ToHidden2.txt";
const char *NeuralNetworkAIPlayer::_pathHidden2ToOutput = ":NeuralNetworkAIPlayer/hidden2ToOutput.txt";

namespace {
float sigmoid(float input){
    return 1 / (1 + exp(-input));
}
}

NeuralNetworkAIPlayer::NeuralNetworkAIPlayer(QObject *parent) :
    Player(parent),
    _inputToHidden1(),
    _hidden1ToHidden2(),
    _hidden2ToOutput()
{

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    for(int i = 0; i < 64; ++i)
    {
        _lastboard[i] = 0;
    }

    // Initialize _inputToHidden1
    QFile ith1(_pathInputToHidden1);
    ith1.open(QIODevice::ReadOnly);
    QTextStream ith1Stream(&ith1);
    for(int i = 0; i < _hiddenSize*129; ++i)
    {
        ith1Stream >> _inputToHidden1(i/_hiddenSize, i%_hiddenSize);
    }
    ith1.close();

    // Initialize _hidden1ToHidden2
    QFile h1th2(_pathHidden1ToHidden2);
    h1th2.open(QIODevice::ReadOnly);
    QTextStream h1th2Stream(&h1th2);
    for(int i = 0; i < _hiddenSize*(_hiddenSize+1); ++i)
    {
        h1th2Stream >> _hidden1ToHidden2(i/_hiddenSize, i%_hiddenSize);
    }
    h1th2.close();

    // Initialize _hidden2ToOutput
    QFile h2to(_pathHidden2ToOutput);
    h2to.open(QIODevice::ReadOnly);
    QTextStream hto2Stream(&h2to);
    for(int i = 0; i < 64*(_hiddenSize+1); ++i)
    {
        hto2Stream >> _hidden2ToOutput(i/64, i%64);
    }
    h2to.close();
}

bool NeuralNetworkAIPlayer::isHuman()
{
    return false;
}

void NeuralNetworkAIPlayer::doTurn(Gameboard board, int player)
{
    QGenericMatrix<129,1,float> input;

    // Current board
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

    // Last board
    for(int i = 64; i < 128; ++i)
    {
        input(0,i) = _lastboard[i-64];
        _lastboard[i-64] = input(0,i-64);
    }

    // Bias
    input(0,128) = 1;

    // Hidden 1
    QGenericMatrix<_hiddenSize,1,float> hidden1 = input * _inputToHidden1;
    QGenericMatrix<_hiddenSize+1,1,float> hidden1_input;
    for(int i = 0; i < _hiddenSize; ++i)
    {
        if(hidden1(0,i) > 0)
        {
            hidden1_input(0,i) = 1;
        }
        else if(hidden1(0,i) < 0)
        {
            hidden1_input(0,i) = -1;
        }
        else
        {
            hidden1_input(0,i) = 0;
        }
    }
    hidden1_input(0,_hiddenSize) = 1;

    // Hidden 2
    QGenericMatrix<_hiddenSize,1,float> hidden2 = hidden1_input * _hidden1ToHidden2;
    QGenericMatrix<_hiddenSize+1,1,float> hidden2_input;
    for(int i = 0; i < _hiddenSize; ++i)
    {
        hidden2_input(0,i) = sigmoid(hidden2(0,i));
    }
    hidden2_input(0,_hiddenSize) = 1;

    // Output

    QGenericMatrix<64,1,float> output = hidden2_input *_hidden2ToOutput;

    float max = -1000000000;
    int turn_save = -1;
    for(int i = 0; i < 64; ++i)
    {
        float value = output(0,i);
        switch(qrand()%3)
        {
        case 1:
            value *= random_fachtor;
            break;
        case 2:
            value /= random_fachtor;
            break;
        default:
            break;
        }

        if(value > max && board.play(i%8, i/8,player,true))
        {
            max = value;
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
