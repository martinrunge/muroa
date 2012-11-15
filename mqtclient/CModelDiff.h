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
	E_AVAIL_RENDER_CLIENT,
	E_OWN_RENDER_CLIENT,
	E_EXTERN,
	E_NONE
};


class comb_hash {
public:
	comb_hash() {
		type = 0;
		hash = 0;
		pl_id = 0;
		path = "";
		line = -1;
	};

	uint32_t type;
	uint32_t hash;
	uint32_t pl_id;
	std::string path;
	int line;
};

typedef struct comb_hash comb_hash_t;

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


	enum origin getCommandType() const { return m_commandType; };
	void setCommandType(const enum origin commandType) { m_commandType = commandType; };


    int getNumSelected() const { return m_selectedItems.size(); };

    int getNumToInsert() const { return m_numToInsert; };
    void setNumToInsert(int numIns) { m_numToInsert = numIns; };
    int getNumToRemove() const { return m_numToRemove; };
    void setNumToRemove(int numRem) { m_numToRemove = numRem; };


    std::vector<comb_hash_t> getSelectedItems() const { return m_selectedItems; }
    void setSelectedItems(std::vector<comb_hash_t> selectedItems) { m_selectedItems = selectedItems; }

    void appendToSelectedItems(comb_hash_t combhash)
    {
    	m_selectedItems.push_back(combhash);
    }

    void dump();

    void sort();

private:

    std::vector<comb_hash_t> m_selectedItems;

	int m_insertPos;

	int m_numToInsert;
	int m_numToRemove;

	enum origin m_origin;
	enum origin m_destination;

	enum origin m_commandType;
};

#endif /* CMODELDIFF_H_ */
