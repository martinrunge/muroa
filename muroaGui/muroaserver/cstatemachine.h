#ifndef CSTATEMACHINE_H
#define CSTATEMACHINE_H

#include <QObject>

class QTcpSocket;
class QXmlStreamReader;
class QXmlStreamWriter;
class CCollection;

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

    inline void setCollection(CCollection* collection) { m_collection = collection; };


    void sendCollection();


private:
    int m_state;
    int m_xml_depth;

    int m_revision;

    CCollection* m_collection;

    QXmlStreamWriter* m_xml_writer;

    void parseReadArgs(QXmlStreamReader* reader);
    void parseWriteArgs(QXmlStreamReader* reader);

};

#endif // CSTATEMACHINE_H
