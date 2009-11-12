#ifndef CSTATEMACHINE_H
#define CSTATEMACHINE_H

#include <QObject>

class QTcpSocket;
class QXmlStreamReader;
class QXmlStreamWriter;
class CCollection;
class CConnection;
class CSession;

class CStateMachine : public QObject
{
    Q_OBJECT;
public:
    CStateMachine(CConnection* connection);

    ~CStateMachine();
    inline void setXmlWriter(QXmlStreamWriter* xml_writer) { m_xml_writer = xml_writer; };


public slots:
    void close();
    void open();

    void addData(QByteArray data);

    void startDocument(QXmlStreamReader* reader);
    void endDocument(QXmlStreamReader* reader);

    void startElement(QXmlStreamReader* reader);
    void endElement(QXmlStreamReader* reader);
    void characters(QXmlStreamReader* reader);

    // inline void setCollection(CCollection* collection) { m_collection = collection; };
    inline void setSessionPtr(CSession* session) { m_session = session; };


    // void sendCollection();


private:
    int m_state;

    QXmlStreamReader* m_xml_reader;
    int m_xml_depth;

    // int m_revision;

    int m_knownRevision;

    CConnection* m_connection;
    CSession* m_session;

    QXmlStreamWriter* m_xml_writer;

    void parseReadArgs(QXmlStreamReader* reader);
    void parseWriteArgs(QXmlStreamReader* reader);

    void parseGetCollectionArgs(QXmlStreamReader* reader);


};

#endif // CSTATEMACHINE_H
