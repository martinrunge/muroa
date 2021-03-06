/*
 * CModelDiff.cpp
 *
 *  Created on: 10 Jan 2010
 *      Author: martin
 */

#include "CModelDiff.h"
#include <QtAlgorithms>
#include <QDebug>

CModelDiff::CModelDiff(enum origin orig) : m_insertPos(-1), m_origin(orig)
{

}

CModelDiff::CModelDiff(const CModelDiff& other)
{
	m_selectedIndexes = other.getSelectedIndexes();
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
	ds << (unsigned)m_commandType;
	ds << m_insertPos;
	ds << m_selectedIndexes;

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

