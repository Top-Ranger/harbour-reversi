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

#include "assemblyaiplayer.h"

//Cores
#include "AssemblyAIPlayer/insanecore.h"

#include <QDebug>
#include <QTime>

AssemblyAIPlayer::AssemblyAIPlayer(QObject *parent) :
    Player(parent),
    _activeCore(NULL)
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    _vote = new float*[8];
    for(int i = 0; i < 8; ++i)
    {
        _vote[i] = new float[8];
    }
    qCritical() << "111";
    _inactiveCores.append(new InsaneCore());
    _inactiveCores.append(new InsaneCore());
    qCritical() << "222";

    int activeCoreIndex = qrand()%_inactiveCores.length();
    _activeCore = _inactiveCores[activeCoreIndex];
    _inactiveCores.removeAt(activeCoreIndex);
    qCritical() << "333";
}

AssemblyAIPlayer::~AssemblyAIPlayer()
{
    qCritical() << "444";
    for(int i = 0; i < 8; ++i)
    {
        delete [] _vote[i];
    }
    delete [] _vote;
    qCritical() << "555";

    for(int i = 0; i < _inactiveCores.length(); ++i)
    {
        qCritical() << "666" << i;
        delete _inactiveCores[i];
    }

    if(_activeCore != NULL)
    {
        delete _activeCore;
    }
}

void AssemblyAIPlayer::doTurn()
{
    emit wantBoard();
}

bool AssemblyAIPlayer::isHuman()
{
    return false;
}

void AssemblyAIPlayer::getBoard(Gameboard board, int player)
{
    int changes = 0;
    bool test = true;
    do {
        if(_activeCore->retirement(board, player))
        {
            Core * temp = _activeCore;
            int newCore = qrand()%_inactiveCores.length();
            _activeCore = _inactiveCores[newCore];
            _inactiveCores[newCore] = temp;
        }
        else
        {
            QList<Core *> wantChange;

            for(int x = 0; x < 8; ++x)
            {
                for(int y = 0; y < 8; ++y)
                {
                    _vote[x][y] = 0;
                }
            }

            _activeCore->propose(_vote, board, player);

            for(int i = 0; i < _inactiveCores.length(); ++i)
            {
                if(_inactiveCores[i]->mistrust(_vote, board, player))
                {
                    wantChange.append(_inactiveCores[i]);
                }
            }

            test = wantChange.length() >= _neededToChange;
            if(test)
            {
                Core * temp = _activeCore;
                int newCore = qrand()%wantChange.length();
                _activeCore = wantChange[newCore];
                int index = _inactiveCores.indexOf(_activeCore);
                if(index == -1)
                {
                    qCritical() << "FATAL ERROR in " __FILE__ << " " << __LINE__ << ": Core not found in _inactiveCores, adding Core to list";
                    _inactiveCores.append(temp);
                }
                else
                {
                    _inactiveCores.replace(index, temp);
                }
            }
        }
    } while(test && (changes < _maxChanges));

    for(int i = 0; i < _inactiveCores.length(); ++i)
    {
        _inactiveCores[i]->correct(_vote, board, player);
    }

    int xTurn = 0;
    int yTurn = 0;
    float max = -1048576;

    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            if(_vote[x][y] > max)
            {
                max = _vote[x][y];
                xTurn = x;
                yTurn = y;
            }
        }
    }

    emit turn(xTurn, yTurn);
}

void AssemblyAIPlayer::humanInput(int x, int y)
{
}
