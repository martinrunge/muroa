/*
 * CModelDiff.cpp
 *
 *  Created on: 10 Jan 2010
 *      Author: martin
 */

#include "CModelDiff.h"
#include <QtAlgorithms>
#include <QDebug>

#include <IContentItem.h>
#include <CRootItem.h>

using namespace std;

CModelDiff::CModelDiff(enum origin orig) : m_insertPos(-1), m_origin(orig)
{

}

CModelDiff::CModelDiff(const CModelDiff& other)
{
	m_selectedItems = other.getSelectedItems();
	m_origin = other.getOrigin();
	m_destination = other.getDestination();

	m_commandType = other.getCommandType();

	m_insertPos = other.getInsertPos();

	m_numToInsert = other.getNumToInsert();
	m_numToRemove = other.getNumToRemove();
}

CModelDiff::CModelDiff(const QByteArray& ba)
{
	QDataStream ds( ba );
	unsigned orig, dest, commandType;
	ds >> orig;
	m_origin = (enum origin)orig;
	ds >> dest;
	m_destination = (enum origin)dest;
	ds >> commandType;
	m_commandType = (enum origin)commandType;

	ds >> m_insertPos;

	int numItems;
	ds >> numItems;
	for(int i = 0; i < numItems; i++) {
		comb_hash_t combhash;
		ds >> combhash.type;
		ds >> combhash.hash;
		quint32 pathsize;
		char* data;
		ds.readBytes(data, pathsize);
		combhash.path = string(data, pathsize);
		delete [] data;
		ds >> combhash.line;
		m_selectedItems.push_back(combhash);
	}
}

CModelDiff::~CModelDiff() {
	// TODO Auto-generated destructor stub
}

QByteArray CModelDiff::toQByteArray() const
{
	QByteArray ba;
	QDataStream ds( &ba, QIODevice::WriteOnly );

	ds << (unsigned)m_origin;
	ds << (unsigned)m_destination;
	ds << (unsigned)m_commandType;
	ds << m_insertPos;

	unsigned numSelectedItems = m_selectedItems.size();
	ds << numSelectedItems;
	for(vector<comb_hash_t>::const_iterator it = m_selectedItems.begin(); it != m_selectedItems.end(); it++ ) {
		ds << it->type;
		ds << it->hash;
		uint32_t pathsize = it->path.size();
		ds.writeBytes(it->path.data(), pathsize);
		ds << it->line;
	}
	return ba;
}

void CModelDiff::sort()
{
	// qSort(m_selectedIndexes.begin(), m_selectedIndexes.end());
}


void CModelDiff::dump() {
	qDebug() << QString("##########  CModelDiff::dump  ###########");
	qDebug() << QString(" %1 item from %2 to %3 pos %4").arg(m_numToRemove).arg(m_origin).arg(m_destination).arg(m_insertPos);
	for(int i = 0; i < m_selectedItems.size(); i++)
	{
		qDebug() << QString(" selected hash: %1 ").arg(m_selectedItems[i].hash);
	}
	qDebug() << QString("##########  CModelDiff::dump  ###########");
}

