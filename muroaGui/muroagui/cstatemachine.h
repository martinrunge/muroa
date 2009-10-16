#ifndef CCONTENTHANDLER_H
#define CCONTENTHANDLER_H


#include <QString>
#include <QObject>

class QTcpSocket;
class QXmlStreamReader;
class QXmlStreamWriter;

class CStateMachine : public QObject
{
    Q_OBJECT;
public:
    CStateMachine();
    ~CStateMachine();

    inline void setXmlWriter(QXmlStreamWriter* xml_writer) { m_xml_writer = xml_writer; };

public slots:
    void close();
    void open();

    void startDocument(QXmlStreamReader* reader);
    void endDocument(QXmlStreamReader* reader);

    void startElement(QXmlStreamReader* reader);
    void endElement(QXmlStreamReader* reader);
    void characters(QXmlStreamReader* reader);


private:
    int m_state;
    int m_xml_depth;

    quint64 m_revision;

    QXmlStreamWriter* m_xml_writer;

    void readCollection(QXmlStreamReader* reader = 0);

    void parseReadArgs(QXmlStreamReader* reader);
    void parseWriteArgs(QXmlStreamReader* reader);

};

#endif // CCONTENTHANDLER_H
