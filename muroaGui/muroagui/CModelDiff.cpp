/*
 * CModelDiff.cpp
 *
 *  Created on: 10 Jan 2010
 *      Author: martin
 */

#include "CModelDiff.h"

CModelDiff::CModelDiff(enum origin orig) : m_insertPos(-1), m_origin(orig)
{

}

CModelDiff::CModelDiff(const CModelDiff& other)
{
	m_selectedIndexes = other.getSelectedIndexes();
	m_origin = other.getOrigin();
	m_destination = other.getDestination();

	m_insertPos = other.getInsertPos();

	m_numToInsert = other.getNumToInsert();
	m_numToRemove = other.getNumToRemove();
}

CModelDiff::CModelDiff(const QByteArray& ba)
{
	QDataStream ds( ba );
	unsigned orig, dest;
	ds >> orig;
	m_origin = (enum origin)orig;
	ds >> dest;
	m_destination = (enum origin)dest;
	ds >> m_insertPos;
	ds >> m_selectedIndexes;
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
	ds << m_insertPos;
	ds << m_selectedIndexes;

	return ba;
}

void CModelDiff::sort()
{
	// m_rowsToRemove.sort();
	// m_rowsToInsert.sort();
}

