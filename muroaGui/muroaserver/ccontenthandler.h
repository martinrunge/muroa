#ifndef CCONTENTHANDLER_H
#define CCONTENTHANDLER_H

#include <QXmlContentHandler>
#include <QString>

class CContentHandler : public QXmlContentHandler
{
public:
    CContentHandler();
    ~CContentHandler();
    bool characters ( const QString & ch );
    bool endDocument ();
    bool endElement ( const QString & namespaceURI, const QString & localName, const QString & qName );
    bool endPrefixMapping ( const QString & prefix );
    QString errorString () const;
    bool ignorableWhitespace ( const QString & ch );
    bool processingInstruction ( const QString & target, const QString & data );
    void setDocumentLocator ( QXmlLocator * locator );
    bool skippedEntity ( const QString & name );
    bool startDocument ();
    bool startElement ( const QString & namespaceURI, const QString & localName, const QString & qName, const QXmlAttributes & atts );
    bool startPrefixMapping ( const QString & prefix, const QString & uri );
};

#endif // CCONTENTHANDLER_H
