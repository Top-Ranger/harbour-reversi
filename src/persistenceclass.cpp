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

#include "persistenceclass.h"
#include "core/commons.h"
#include <QXmlStreamReader>
#include <QStandardPaths>
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
                        REVERSI_WARNING_MSG("Reading - Can not determine attributes of " << reader.name());
                    }
                }
                else
                {
                    REVERSI_WARNING_MSG("Reading - Unknown Element: " << reader.name());
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
                REVERSI_WARNING_MSG("Reading -" << reader.errorString());
                file.close();
                return;

            default:
                REVERSI_WARNING_MSG("Reading - Unknown token:" << reader.tokenString());
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
            REVERSI_DEBUG_MSG("Creating directory" << path);
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
                REVERSI_WARNING_MSG("Writing - Error while writing file");
            }

            file.close();
        }
        else
        {
            REVERSI_WARNING_MSG("Writing - Can not write file \"" << path << "\":" << file.errorString());
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
