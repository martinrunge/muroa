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

	enum origin getDestination() const { return m_destination; };
	void setDestination(const enum origin dest) { m_destination = dest; };

    int getNumSelected() const { return m_selectedIndexes.size(); };

    int getNumToInsert() const { return m_numToInsert; };
    void setNumToInsert(int numIns) { m_numToInsert = numIns; };
    int getNumToRemove() const { return m_numToRemove; };
    void setNumToRemove(int numRem) { m_numToRemove = numRem; };


    QList<unsigned> getSelectedIndexes() const { return m_selectedIndexes; }
    void setSelectedIndexes(QList<unsigned > selectedIndexes) { m_selectedIndexes = selectedIndexes; }

    void appendToSelectedIndexes(unsigned rowIndex)
    {
    	m_selectedIndexes.append(rowIndex);
    }


    void sort();

private:
	QList<unsigned> m_selectedIndexes;

	int m_insertPos;

	int m_numToInsert;
	int m_numToRemove;

	enum origin m_origin;
	enum origin m_destination;
};

#endif /* CMODELDIFF_H_ */
