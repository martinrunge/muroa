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
#include <vector>

enum origin {
	E_MEDIA_COL,
	E_PLAYLIST,
	E_NEXTLIST,
	E_EXTERN,
	E_NONE
};

class CItemBase;
class CRootItem;

class CModelDiff {
public:
	CModelDiff(enum origin orig = E_NONE);
	CModelDiff(const CModelDiff& other);
	CModelDiff(const QByteArray& ba, CRootItem* originRi, CRootItem* destinationRi);

	virtual ~CModelDiff();

	QByteArray toQByteArray() const;

	int getInsertPos() const { return m_insertPos; };
	void setInsertPos(const int pos) { m_insertPos = pos; };

	enum origin getOrigin() const { return m_origin; };
	void setOrigin(const enum origin orig) { m_origin = orig; };

	enum origin getDestination() const { return m_destination; };
	void setDestination(const enum origin dest) { m_destination = dest; };

//	CRootItem* getOriginRoot(void) { return m_originRoot; };
//	void setOriginRoot(const CRootItem* ri) { m_originRoot = ri; };
//
//	CRootItem* getDestinationRoot(void) {return m_destinationRoot; };
//	void setDestinationRoot(CRootItem* ri) { m_destinationRoot = ri; };


	enum origin getCommandType() const { return m_commandType; };
	void setCommandType(const enum origin commandType) { m_commandType = commandType; };


    int getNumSelected() const { return m_selectedItems.size(); };

    int getNumToInsert() const { return m_numToInsert; };
    void setNumToInsert(int numIns) { m_numToInsert = numIns; };
    int getNumToRemove() const { return m_numToRemove; };
    void setNumToRemove(int numRem) { m_numToRemove = numRem; };


    std::vector<CItemBase*> getSelectedItems() const { return m_selectedItems; }
    void setSelectedItems(std::vector<CItemBase*> selectedItems) { m_selectedItems = selectedItems; }

    void appendToSelectedItems(CItemBase* itemPtr)
    {
    	m_selectedItems.push_back(itemPtr);
    }

    void dump();

    void sort();

private:
	std::vector<CItemBase*> m_selectedItems;

	int m_insertPos;

	int m_numToInsert;
	int m_numToRemove;

	enum origin m_origin;
	enum origin m_destination;

	enum origin m_commandType;

//	CRootItem* m_originRoot;
//	CRootItem* m_destinationRoot;
};

#endif /* CMODELDIFF_H_ */
