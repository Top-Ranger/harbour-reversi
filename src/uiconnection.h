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

#ifndef UICONNECTION_H
#define UICONNECTION_H

#include "persistenceclass.h"

#include <QObject>
#include <QTranslator>

class UIConnection : public QObject
{
    Q_OBJECT
public:
    explicit UIConnection(QTranslator *translator, QTranslator *coreTranslator, QObject *parent = 0);

signals:
    void changedPlayer(QString s);
    void finished();
    void started();
    void configureGameShown();

public slots:
    Q_INVOKABLE void newPlayerMessage(QString s);
    Q_INVOKABLE void endOfGame();
    Q_INVOKABLE void startOfGame();
    Q_INVOKABLE void configureGame();
    Q_INVOKABLE void changeLanguage(QString language);
    Q_INVOKABLE int indexPlayer1();
    Q_INVOKABLE int indexPlayer2();
    Q_INVOKABLE int indexColourPrefix();
    Q_INVOKABLE void setIndexPlayer1(int index);
    Q_INVOKABLE void setIndexPlayer2(int index);
    Q_INVOKABLE void setIndexColourPrefix(int index);
    Q_INVOKABLE int bonus();
    Q_INVOKABLE void setBonus(int bonus);
    Q_INVOKABLE void setStringPlayer1(QString s);
    Q_INVOKABLE void setStringPlayer2(QString s);
    Q_INVOKABLE void setColourPrefix(QString s);
    Q_INVOKABLE QString stringPlayer1();
    Q_INVOKABLE QString stringPlayer2();
    Q_INVOKABLE QString colourPrefix();
    Q_INVOKABLE bool showMessage();
    Q_INVOKABLE void setShowMessage(bool showMessage);

private:
    QTranslator *_translator;
    QTranslator *_coreTranslator;
    bool _running;
    int _indexPlayer1;
    int _indexPlayer2;
    int _indexColourPrefix;
    int _bonus;
    QString _stringPlayer1;
    QString _stringPlayer2;
    QString _colourPrefix;
    PersistenceClass _save;
    bool _showMessage;
};

#endif // UICONNECTION_H
