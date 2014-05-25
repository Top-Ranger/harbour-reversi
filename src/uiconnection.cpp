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

#include "uiconnection.h"
#include <QDebug>

UIConnection::UIConnection(QTranslator *translator, QTranslator *coreTranslator, QObject *parent) :
    QObject(parent),
    _translator(translator),
    _coreTranslator(coreTranslator),
    _running(false),
    _indexPlayer1(0),
    _indexPlayer2(0),
    _bonus(0),
    _stringPlayer1("Human"),
    _stringPlayer2("Human")
{
}

void UIConnection::newPlayerMessage(QString s)
{
    if(_running)
    {
        emit changedPlayer(s);
    }
}

void UIConnection::endOfGame()
{
    emit finished();
    _running = false;
}

void UIConnection::startOfGame()
{
    _running = true;
    emit started();
}

void UIConnection::configureGame()
{
    emit configureGameShown();
}

void UIConnection::changeLanguage(QString language)
{
    if(_coreTranslator != NULL && _translator != NULL)
    {
        _translator->load((QString(":translation/harbour-reversi-ui_%1").arg(language)));
        _coreTranslator->load(QString(":translation/reversi-core_%1").arg(language));
    }
    else
    {
        qCritical() << "FATAL ERROR in " __FILE__ << " " << __LINE__ << ": Trying to change language with NULL-Translator";
    }
}

int UIConnection::indexPlayer1()
{
    return _indexPlayer1;
}

int UIConnection::indexPlayer2()
{
    return _indexPlayer2;
}

void UIConnection::setIndexPlayer1(int index)
{
    _indexPlayer1 = index;
}

void UIConnection::setIndexPlayer2(int index)
{
    _indexPlayer2 = index;
}

int UIConnection::bonus()
{
    return _bonus;
}

void UIConnection::setBonus(int bonus)
{
    _bonus = bonus;
}

void UIConnection::setStringPlayer1(QString s)
{
    _stringPlayer1 = s;
}

void UIConnection::setStringPlayer2(QString s)
{
    _stringPlayer2 = s;
}

QString UIConnection::stringPlayer1()
{
    return _stringPlayer1;
}

QString UIConnection::stringPlayer2()
{
    return _stringPlayer2;
}
