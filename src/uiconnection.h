#ifndef UICONNECTION_H
#define UICONNECTION_H

#include <QObject>

class UIConnection : public QObject
{
    Q_OBJECT
public:
    explicit UIConnection(QObject *parent = 0);

signals:
    void changedPlayer(QString s);
    void finished();
    void started();

public slots:
    Q_INVOKABLE void newPlayerMessage(QString s);
    Q_INVOKABLE void endOfGame();
    Q_INVOKABLE void startOfGame();

private:
    bool _running;

};

#endif // UICONNECTION_H
