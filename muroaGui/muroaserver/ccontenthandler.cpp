#include "ccontenthandler.h"

#include <QDebug>

CContentHandler::CContentHandler()
{
}


CContentHandler::~CContentHandler()
{
}

bool CContentHandler::characters ( const QString & ch )
{
    qDebug() << QString("characters %1").arg(ch);
    return true;
}

bool CContentHandler::endDocument ()
{
        qDebug() << QString("endDocument");
        return true;
}

bool CContentHandler::endElement ( const QString & namespaceURI, const QString & localName, const QString & qName )
{
    qDebug() << QString("endElement %1").arg(qName);
    return true;
}

bool CContentHandler::endPrefixMapping ( const QString & prefix )
{
    qDebug() << QString("endPrefixMapping %1").arg(prefix);
    return true;
}

QString CContentHandler::errorString () const
{
    return QString("myerrorstring");
}

bool CContentHandler::ignorableWhitespace ( const QString & ch )
{
    qDebug() << QString("ignorableWhitespace %1").arg(ch);
    return true;
}

bool CContentHandler::processingInstruction ( const QString & target, const QString & data )
{
    qDebug() << QString("processingInstruction %1 %2").arg(target).arg(data);
    return true;
}

void CContentHandler::setDocumentLocator ( QXmlLocator * locator )
{
    qDebug() << QString("setDocumentLocator");

}

bool CContentHandler::skippedEntity ( const QString & name )
{
    qDebug() << QString("skippedEntity %1").arg(name);
    return true;
}

bool CContentHandler::startDocument()
{
    qDebug() << QString("startDocument");
    return true;
}

bool CContentHandler::startElement ( const QString & namespaceURI, const QString & localName, const QString & qName, const QXmlAttributes & atts )
{
        qDebug() << QString("startElement %1").arg(qName);
        return true;
}

bool CContentHandler::startPrefixMapping ( const QString & prefix, const QString & uri )
{
        qDebug() << QString("startPrefixMapping %1 %2").arg(prefix).arg(uri);
        return true;
}
