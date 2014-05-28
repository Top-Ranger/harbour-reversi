#include "persistenceclass.h"
#include <QXmlStreamReader>
#include <QStandardPaths>
#include <QDebug>
#include <QXmlStreamAttributes>
#include <QXmlStreamWriter>
#include <QFile>
#include <QDir>

PersistenceClass::PersistenceClass(QObject *parent) :
    QObject(parent),
    _hash(),
    _hasChanged(false)
{
    QString path = QString(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation));
    path.append("/harbour-reversi/harbour-reversi.xml");

    QFile file(path);

    if(file.exists() && file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QXmlStreamReader reader(&file);

        while(!reader.atEnd())
        {
            QString key = "";
            QString value = QString();
            switch(reader.readNext())
            {
            case QXmlStreamReader::StartDocument:
                break;

            case QXmlStreamReader::StartElement:
                if(reader.name() == "optionList")
                {
                    break;
                }
                else if(reader.name() == "String")
                {
                    QXmlStreamAttributes attribute = reader.attributes();
                    if(attribute.hasAttribute("key") && attribute.hasAttribute("value"))
                    {
                        key = attribute.value("key").toString();
                        value = attribute.value("value").toString();
                        _hash[key] = value;
                    }
                    else
                    {
                        qWarning() << "WARNING in " __FILE__ << " " << __LINE__ << ": Reading - Can not determine attributes of " << reader.name();
                    }
                }
                else
                {
                    qWarning() << "WARNING in " __FILE__ << " " << __LINE__ << ": Reading - Unknown Element: " << reader.name();
                }
                break;

            case QXmlStreamReader::Comment:
                break;

            case QXmlStreamReader::EndElement:
                break;

            case QXmlStreamReader::Characters:
                break;

            case QXmlStreamReader::EndDocument:
                break;

            case QXmlStreamReader::Invalid:
                qWarning() << "WARNING in " __FILE__ << " " << __LINE__ << ": Reading -" << reader.errorString();
                file.close();
                return;

            default:
                qWarning() << "WARNING in " __FILE__ << " " << __LINE__ << ": Reading - Unknown token:" << reader.tokenString();
            }
        }
        file.close();
    }
}

PersistenceClass::~PersistenceClass()
{
    saveNow();
}

void PersistenceClass::saveNow()
{
    if(_hasChanged)
    {
        QString path = QString(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation));
        path.append("/harbour-reversi/");

        QDir dir(path);
        if(!dir.exists())
        {
            qWarning() << "DEBUG in " __FILE__ << " " << __LINE__ << ": Creating directory" << path;
            dir.mkdir(path);
        }

        path.append("/harbour-reversi.xml");
        QFile file(path);

        if(file.open(QIODevice::WriteOnly))
        {

            QXmlStreamWriter writer(&file);
            writer.setAutoFormatting(true);
            writer.writeStartDocument();
            writer.writeStartElement("optionList");

            for(QHash<QString,QString>::const_iterator i = _hash.begin(); i != _hash.end(); ++i)
            {
                writer.writeStartElement("String");
                writer.writeAttribute("key",i.key());
                writer.writeAttribute("value", i.value());
                writer.writeEndElement();
            }

            writer.writeEndElement();
            writer.writeEndDocument();

            if(writer.hasError())
            {
                qWarning() << "WARNING in " __FILE__ << " " << __LINE__ << ": Writing - Error while writing file";
            }

            file.close();
        }
        else
        {
            qWarning() << "WARNING in " __FILE__ << " " << __LINE__ << ": Writing - Can not write file \"" + path + "\":" + file.errorString();
        }
        _hasChanged = false;
    }
}

QString PersistenceClass::getString(QString s)
{
    return _hash.value(s);
}

void PersistenceClass::saveString(QString s, QString v)
{
    _hash[s] = v;
    _hasChanged = true;
}
