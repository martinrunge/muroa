/*
 * CModelDiff.h
 *
 *  Created on: 10 Jan 2010
 *      Author: martin
 */

#ifndef CMODELDIFF_H_
#define CMODELDIFF_H_

#include <QByteArray>
#include <QModelIndex>


class CModelDiff {
public:
	CModelDiff();
	CModelDiff(const CModelDiff& other);
	CModelDiff(const QByteArray& ba);

	virtual ~CModelDiff();

	QByteArray toQByteArray() const;

    int getNumRowsToInsert() const
    {
        return m_rowsToInsert.size();
    }

    int getNumRowsToRemove() const
    {
        return m_rowsToRemove.size();
    }

    QList<unsigned > getRowsToInsert() const
    {
        return m_rowsToInsert;
    }

    QList<unsigned > getRowsToRemove() const
    {
        return m_rowsToRemove;
    }

    void setRowsToInsert(QList<unsigned > rowsToInsert)
    {
        m_rowsToInsert = rowsToInsert;
    }

    void setRowsToRemove(QList<unsigned > rowsToRemove)
    {
        m_rowsToRemove = rowsToRemove;
    }

    void setRowsToInsert(QModelIndexList rowsToInsert);
    void setRowsToRemove(QModelIndexList rowsToRemove);

    void appendRowToRemove(unsigned row)
    {
    	m_rowsToRemove.append(row);
    }

    void appendRowToInsert(unsigned row)
    {
    	m_rowsToInsert.append(row);
    }


private:
	QList<unsigned> m_rowsToRemove;
	QList<unsigned> m_rowsToInsert;
};

#endif /* CMODELDIFF_H_ */
