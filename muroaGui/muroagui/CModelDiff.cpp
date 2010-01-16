/*
 * CModelDiff.cpp
 *
 *  Created on: 10 Jan 2010
 *      Author: martin
 */

#include "CModelDiff.h"

CModelDiff::CModelDiff()
{
	// TODO Auto-generated constructor stub

}

CModelDiff::CModelDiff(const CModelDiff& other)
{
	m_rowsToRemove = other.getRowsToRemove();
	m_rowsToInsert = other.getRowsToInsert();
}

CModelDiff::CModelDiff(const QByteArray& ba)
{
	QDataStream ds( ba );

	ds >> m_rowsToRemove;
	ds >> m_rowsToInsert;
}

CModelDiff::~CModelDiff() {
	// TODO Auto-generated destructor stub
}

QByteArray CModelDiff::toQByteArray() const
{
	QByteArray ba;
	QDataStream ds( &ba, QIODevice::WriteOnly );

	ds << m_rowsToRemove;
	ds << m_rowsToInsert;

	return ba;
}

void CModelDiff::setRowsToInsert(QModelIndexList rowsToInsert)
{
	m_rowsToInsert.clear();
	for(int i = 0; i < rowsToInsert.size(); i++)
	{
		m_rowsToInsert.append(rowsToInsert.at(i).row());
	}
}

void CModelDiff::setRowsToRemove(QModelIndexList rowsToRemove)
{
	m_rowsToRemove.clear();
	for(int i = 0; i < rowsToRemove.size(); i++)
	{
		m_rowsToRemove.append(rowsToRemove.at(i).row());
	}
}

void CModelDiff::sort()
{
	// m_rowsToRemove.sort();
	// m_rowsToInsert.sort();
}

