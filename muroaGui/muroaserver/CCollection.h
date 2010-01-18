/*
 * CCollection.h
 *
 *  Created on: 16 Oct 2009
 *      Author: martin
 */

#ifndef CCOLLECTION_H_
#define CCOLLECTION_H_

#include <QList>
#include <QString>
#include <QRegExp>
#include "CCollectionItem.h"
#include <QDebug>

template <class T>
class CCollection
{
public:
	CCollection() {} ;
	CCollection(const CCollection<T>& other);
	virtual ~CCollection() {} ;

	inline void setText(QString collection, int revision)
	{
		m_collectionAsText = collection;
		m_revision = revision;
		parse();
	};

	int patch(QString* diff, int revision);

	inline int size() const { return m_items.size(); };
	inline T getItem(int pos) const { return m_items.at(pos); };

	QString getText(void);
	inline int getRevision(void) { return m_revision; };

private:
	QList<T> m_items;

	void parse();

	QString m_collectionAsText;
	int m_revision;
};



template <class T> CCollection<T>::CCollection(const CCollection<T>& other)
{
	m_items = other.m_items;
	m_collectionAsText = other.m_collectionAsText;
	m_revision = other.m_revision;
}

template <class T> void CCollection<T>::parse()
{
	int start(0);
	int end(-1);

	int index;
	//while( (index = m_collectionAsText.indexOf(QChar::LineSeparator, start)) != -1 )
	while( (index = m_collectionAsText.indexOf('\n', start)) != -1 )
	{
		QStringRef line(&m_collectionAsText, start, index - start);
		qDebug() << QString("parsed line: %1") .arg( line.toString());
		T newItem(line.toString());
		m_items.append(newItem);
		start = index + 1;
	}
}

template <class T> QString CCollection<T>::getText()
{
	QString collection;
	qDebug() << QString("CCollection<T>::getText(): m_items.size() = %1").arg(m_items.size());
	for(int i=0; i < m_items.size(); i++)
	{
		QString line = m_items.at(i).getText();
		qDebug() << QString("appending %1").arg(line);
		collection.append(line).append('\n');
	}

	return collection;
}

template <class T> int CCollection<T>::patch(QString* diff, int revision)
{
	qDebug() << QString("CCollection<T>::patch: %1").arg(*diff);
	//template <typename T> void CStateMachine::parseCollectionDiff(QStringRef text, CModelBase<T*>* model)
	QTextStream stream(diff, QIODevice::ReadOnly);
	QString line;

	QRegExp rxdiff("^@@ -(\\d+),(\\d+)\\s+\\+(\\d+),(\\d+)\\s*@@$");

	int oldStart(0);
	int oldLen(0);
	int newStart(0);
	int newLen(0);

	int lineNr(0);
	int chunkSizeSum(0);

	do {
		line = stream.readLine();
		if(line.isEmpty()) continue;
		if(line.startsWith("@@")) // diff chunk header
		{
			int pos = rxdiff.indexIn(line);
			if (pos > -1) {
				QString oldStartStr = rxdiff.cap(1);
				QString oldLenStr = rxdiff.cap(2);
				QString newStartStr = rxdiff.cap(3);
				QString newLenStr = rxdiff.cap(4);
				bool ok;
				oldStart = oldStartStr.toInt(&ok);
				oldLen = oldLenStr.toInt(&ok);
				newStart = newStartStr.toInt(&ok);
				newLen = newLenStr.toInt(&ok);

				if(oldLen == 0) oldStart++;
				lineNr = oldStart + chunkSizeSum;

				chunkSizeSum += newLen - oldLen;
				// qDebug() << QString("- %1,%2 + %3,%4").arg(oldStart).arg(oldLen).arg(newStart).arg(newLen);
			}
		}
		else
		{
			QChar sign = line.at(0);
			QString content = line.right(line.size() - 1);

			switch(sign.unicode()){
				case '+': //insert
				{
					//qDebug() << QString("adding line : %1").arg(lineNr);
					//qDebug() << QString("from diff : %1").arg(content);
					T newItem(content);
					m_items.insert(lineNr - 1, newItem );
					//qDebug() << QString("collection: %1").arg(m_collectionModelPtr->getItemAsString(lineNr - 1));
					break;
				}
				case '-': //remove
					if(content.compare(m_items.at(lineNr - 1).getText()) != 0 )	// possible error:
					{
						qDebug() << QString("Error when removing line %1:").arg(lineNr);
						qDebug() << QString("line expected from diff : %1").arg(content);
						qDebug() << QString("differs form collection : %1").arg(m_items.at(lineNr - 1).getText());
					}
					m_items.removeAt(lineNr - 1);
					lineNr--;
					break;
				case ' ': //check
					if(content.compare(m_items.at(lineNr - 1).getText()) != 0 )	// possible error:
					{
						qDebug() << QString("Error when keeping line %1:").arg(lineNr);
						qDebug() << QString("line expected from diff : %1").arg(content);
						qDebug() << QString("differs from collection : %1").arg(m_items.at(lineNr - 1).getText());
					}
					break;
				default:
					break;

			}
			lineNr++;
		}
		// check if line stays equal, is added or removed

	} while (!line.isNull());

}


#endif /* CCOLLECTION_H_ */
