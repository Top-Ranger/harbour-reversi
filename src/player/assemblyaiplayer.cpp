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
#include "AssemblyAIPlayer/assemblyaihelper.h"

//Cores
#include "AssemblyAIPlayer/insanecore.h"
#include "AssemblyAIPlayer/cornercore.h"
#include "AssemblyAIPlayer/centercore.h"
#include "AssemblyAIPlayer/greedycore.h"
#include "AssemblyAIPlayer/edgecore.h"
#include "AssemblyAIPlayer/frontierdiscscore.h"
#include "AssemblyAIPlayer/movementcore.h"
#include "AssemblyAIPlayer/freemovecore.h"

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

    // To include a new Core you simply have to append it here to the list
    // Please keep in mind: You should never have more than one of each cores in the list - or else you might get problems with the way the cores do comparison
    _inactiveCores.append(new InsaneCore());
    _inactiveCores.append(new CornerCore());
    _inactiveCores.append(new CenterCore());
    _inactiveCores.append(new GreedyCore());
    _inactiveCores.append(new EdgeCore());
    _inactiveCores.append(new FrontierDiscsCore());
    _inactiveCores.append(new MovementCore());
    _inactiveCores.append(new FreeMoveCore());

    int activeCoreIndex = qrand()%_inactiveCores.length();
    _activeCore = _inactiveCores[activeCoreIndex];
    _inactiveCores.removeAt(activeCoreIndex);
}

AssemblyAIPlayer::~AssemblyAIPlayer()
{
    for(int i = 0; i < 8; ++i)
    {
        delete [] _vote[i];
    }
    delete [] _vote;

    for(int i = 0; i < _inactiveCores.length(); ++i)
    {
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
    QString message;
    int changes = 0;
    bool test = true;
    message = QString();
    bool proposal = false;

    for(int x = 0; x < 8; ++x)
    {
        for(int y = 0; y < 8; ++y)
        {
            _vote[x][y] = 0;
        }
    }

    do {
        proposal = false;
        message.append(QString(tr("%1 is the current active core.\n")).arg(_activeCore->name()));
        if(_activeCore->retirement(board, player))
        {
            ++changes;
            message.append(QString(tr("%1 retires.\n")).arg(_activeCore->name()));
            Core * temp = _activeCore;
            int newCore = qrand()%_inactiveCores.length();
            _activeCore = _inactiveCores[newCore];
            _inactiveCores[newCore] = temp;
            message.append(QString(tr("Electing a new active core.\n")));
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
            proposal = true;
            message.append(QString(tr("%1 makes a proposal.\n")).arg(_activeCore->name()));

            for(int i = 0; i < _inactiveCores.length(); ++i)
            {
                if(_inactiveCores[i]->mistrust(_vote, board, player))
                {
                    message.append(QString(tr("%1 doesn't agree.\n")).arg(_inactiveCores[i]->name()));
                    wantChange.append(_inactiveCores[i]);
                }
                else
                {
                    message.append(QString(tr("%1 agrees.\n")).arg(_inactiveCores[i]->name()));
                }
            }

            test = wantChange.length() >= _neededToChange;
            if(test)
            {
                ++changes;
                message.append(QString(tr("To many have disagreed. Electing a new active core.\n")));
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

    if(!proposal)
    {
        for(int x = 0; x < 8; ++x)
        {
            for(int y = 0; y < 8; ++y)
            {
                _vote[x][y] = 0;
            }
        }
        _activeCore->propose(_vote, board, player);
    }

    for(int i = 0; i < _inactiveCores.length(); ++i)
    {
        _inactiveCores[i]->correct(_vote, board, player);
    }

    AssemblyAI::ensureNoIllegalMove(_vote, board, player);

    int xTurn = -1;
    int yTurn = -1;
    float max = 0;

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

    if(changes < _maxChanges)
    {
        message.append(tr("A solution has been found."));
    }
    else
    {
        message.append(tr("Too many new elections, now using %1 to find a solution.\nNo good solution has been found, using the next available.").arg(_activeCore->name()));
    }

    if(xTurn == -1 || yTurn == -1 || board.play(xTurn, yTurn, player, true) == false)
    {
        message.append(tr("\nSomething went wrong - using emergancy plan."));
        int x = qrand()%8;
        int y = qrand()%8;
        int xstart = x;
        int ystart = y;

        do
        {
            do
            {
                if(board.play(x,y,player,true))
                {
                    emit sendMessage(message);
                    emit turn(x,y);
                    return;
                }
                y = (y+1)%8;
            }while(y != ystart);

            x = (x+1)%8;
        }while(x != xstart);
    }
    else
    {
        emit sendMessage(message);
        emit turn(xTurn, yTurn);
    }
}

void AssemblyAIPlayer::humanInput(int x, int y)
{
}
