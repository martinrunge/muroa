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

#include <assert.h>

enum origin {
	E_COLLECTION,
	E_PLAYLIST,
	E_NEXTLIST,
	E_EXTERN,
	E_NONE
};

class CModelDiff {
public:
	CModelDiff(enum origin orig = E_NONE);
	CModelDiff(const CModelDiff& other);
	CModelDiff(const QByteArray& ba);

	virtual ~CModelDiff();

	QByteArray toQByteArray() const;

	int getInsertPos() const { return m_insertPos; };
	void setInsertPos(const int pos) { m_insertPos = pos; };

	enum origin getOrigin() const { return m_origin; };
	void setOrigin(const enum origin orig) { m_origin = orig; };


    int getNumToInsert() const
    {
    	int numIdx = m_indexesToInsert.size();
    	int numRow = m_hashesToInsert.size();
    	assert(numIdx == numRow);
        return numIdx;
    }

    int getNumToRemove() const
    {
    	int numIdx = m_indexesToRemove.size();
    	int numRow = m_hashesToRemove.size();
    	assert(numIdx == numRow);
        return numIdx;
    }

    QList<unsigned> getIndexesToInsert() const { return m_indexesToInsert; }
    QList<unsigned>  getHashesToInsert() const { return m_hashesToInsert; }
    QList<unsigned> getIndexesToRemove() const { return m_indexesToRemove; }
    QList<unsigned>  getHashesToRemove() const { return m_hashesToRemove; }

    void setIndexesToInsert(QList<unsigned > indexesToInsert) { m_indexesToInsert = indexesToInsert; }
    void setIndexesToRemove(QList<unsigned > indexesToRemove) { m_indexesToRemove = indexesToRemove; }

    void appendToRemove(unsigned rowIndex, unsigned hash)
    {
    	m_indexesToRemove.append(rowIndex);
    	m_hashesToRemove.append(hash);
    }

    void appendToInsert(unsigned rowIndex, unsigned hash)
    {
    	m_indexesToInsert.append(rowIndex);
    	m_hashesToInsert.append(hash);
    }

    void sort();

private:
	QList<unsigned> m_indexesToRemove;
	QList<unsigned> m_hashesToRemove;

	QList<unsigned> m_indexesToInsert;
	QList<unsigned> m_hashesToInsert;

	int m_insertPos;
	enum origin m_origin;
};

#endif /* CMODELDIFF_H_ */
