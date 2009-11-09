#ifndef CCONTENTHANDLER_H
#define CCONTENTHANDLER_H


#include <QString>
#include <QObject>

class QTcpSocket;
class QXmlStreamReader;
class QXmlStreamWriter;
class CCollectionModel;

enum states { e_no_session,
			  e_session_active,
			  e_awaiting_collection,
			  e_reading_collection,
			  e_collection_received  };

class CStateMachine : public QObject
{
    Q_OBJECT;
public:
    CStateMachine();
    ~CStateMachine();

    inline void setXmlWriter(QXmlStreamWriter* const xml_writer) { m_xml_writer = xml_writer; };
    inline void setCollectionModelPtr(CCollectionModel* const collectionModelPtr ) { m_collectionModelPtr = collectionModelPtr; };

    inline int getRevision() { return m_revision; };

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

    CCollectionModel* m_collectionModelPtr;

    int m_revision;

    QXmlStreamWriter* m_xml_writer;

    void readCollection(QXmlStreamReader* reader = 0);
    void parseCollection(QStringRef text);

    void parseReadArgs(QXmlStreamReader* reader);
    void parseWriteArgs(QXmlStreamReader* reader);

};

#endif // CCONTENTHANDLER_H
