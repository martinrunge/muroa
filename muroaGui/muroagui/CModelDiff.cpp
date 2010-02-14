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
	m_indexesToRemove = other.getIndexesToRemove();
	m_hashesToRemove = other.getHashesToRemove();
	m_indexesToInsert = other.getIndexesToInsert();
	m_hashesToInsert = other.getHashesToInsert();
}

CModelDiff::CModelDiff(const QByteArray& ba)
{
	QDataStream ds( ba );
	unsigned orig;
	ds >> orig;
	m_origin = (enum origin)orig;
	ds >> m_insertPos;
	ds >> m_indexesToRemove;
	ds >> m_hashesToRemove;
	ds >> m_indexesToRemove;
	ds >> m_hashesToInsert;
}

CModelDiff::~CModelDiff() {
	// TODO Auto-generated destructor stub
}

QByteArray CModelDiff::toQByteArray() const
{
	QByteArray ba;
	QDataStream ds( &ba, QIODevice::WriteOnly );

	ds << (unsigned)m_origin;
	ds << m_insertPos;
	ds << m_indexesToRemove;
	ds << m_hashesToRemove;
	ds << m_indexesToRemove;
	ds << m_hashesToInsert;

	return ba;
}

void CModelDiff::sort()
{
	// m_rowsToRemove.sort();
	// m_rowsToInsert.sort();
}

