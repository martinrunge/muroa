/*
 * CModelDiff.cpp
 *
 *  Created on: 10 Jan 2010
 *      Author: martin
 */

#include "CModelDiff.h"
#include <QtAlgorithms>
#include <QDebug>

#include <CItemBase.h>

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

CModelDiff::CModelDiff(const QByteArray& ba, CRootItem* originRi, CRootItem* destinationRi)
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
	ds << numItems;
	for(int i = 0; i < numItems; i++) {
		int type;
		ds >> type;
		if(type != CItemType::E_CAT) {
			string path;
			ds >> QString(path.c_str());
			CCategoryItem* catIt = originRi->getCategoryPtr(path);
			m_selectedItems.push_back(catIt);
		}
		else {
			uint32_t hash;
			ds >> hash;
			IContentItem* contentIt = originRi->getContentPtr(type, hash);
			m_selectedItems.push_back(contentIt);
		}
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

	ds << m_selectedItems.size();
	for(vector<CItemBase*>::iterator it = m_selectedItems.begin(); it != m_selectedItems.end(); it++ ) {
		if((*it)->type() != CItemType::E_CAT ) {
			IContentItem* contentIt = reinterpret_cast<IContentItem*>(*it);
			ds << (*it)->type();
			ds << contentIt->getHash();
		}
		else {
			CCategoryItem* catIt = reinterpret_cast<CCategoryItem*>(*it);
			ds << (*it)->type();
			ds << catIt->getPath();
		}
	}
	return ba;
}

void CModelDiff::sort()
{
	qSort(m_selectedIndexes.begin(), m_selectedIndexes.end());
}


void CModelDiff::dump() {
	qDebug() << QString("##########  CModelDiff::dump  ###########");
	qDebug() << QString(" %1 item from %2 to %3 pos %4").arg(m_numToRemove).arg(m_origin).arg(m_destination).arg(m_insertPos);
	for(int i = 0; i < m_selectedIndexes.size(); i++)
	{
		qDebug() << QString(" selected index: %1 ").arg(m_selectedIndexes.at(i));
	}
	qDebug() << QString("##########  CModelDiff::dump  ###########");
}

