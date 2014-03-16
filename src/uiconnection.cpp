#include "uiconnection.h"

UIConnection::UIConnection(QObject *parent) :
    QObject(parent),
    _running(false)
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
    _running = false;
    emit finished();
}

void UIConnection::startOfGame()
{
    _running = true;
    emit started();
}
