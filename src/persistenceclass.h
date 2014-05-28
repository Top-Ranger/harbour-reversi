#ifndef PERSISTENCECLASS_H
#define PERSISTENCECLASS_H

#include <QObject>
#include <QHash>

class PersistenceClass : public QObject
{
    Q_OBJECT
public:
    explicit PersistenceClass(QObject *parent = 0);
    ~PersistenceClass();

signals:

public slots:
    void saveNow();
    QString getString(QString s);
    void saveString(QString s, QString v);

private:
    QHash<QString,QString> _hash;
    bool _hasChanged;
};

#endif // PERSISTENCECLASS_H
