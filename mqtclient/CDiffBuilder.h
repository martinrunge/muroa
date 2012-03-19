/*
 * CDiffBuilder.h
 *
 *  Created on: 16 Feb 2010
 *      Author: martin
 */

#ifndef CDIFFBUILDER_H_
#define CDIFFBUILDER_H_

#include <QObject>
#include <QDebug>

#include <CRootItem.h>
#include "CModelDiff.h"
#include "cmds/CmdBase.h"

class CCollectionItem;
class CPlaylistItem;


class CDiffBuilder : public QObject {
	Q_OBJECT;

public:
	CDiffBuilder(CRootItem* mediaColPtr, CRootItem* plPtr, CRootItem* nlPtr );
	virtual ~CDiffBuilder();

	std::string diff(CModelDiff md);

signals:
	void sendCommand(CmdBase* cmd);


private:
	CRootItem* m_mediaColPtr;
	CRootItem* m_plPtr;
	CRootItem* m_nlPtr;

	typedef std::string (CDiffBuilder::*getItemPtr)(comb_hash_t);

	CDiffBuilder::getItemPtr getItemToRemove;
	CDiffBuilder::getItemPtr getItemToInsert;

//	std::string (CDiffBuilder::*getItemToRemove)(int);
//	std::string (CDiffBuilder::*getItemToInsert)(int);

	void prepareDiff(CModelDiff* md);
	std::string prepareDiffHeader(unsigned minusPos, unsigned minusNum, unsigned plusPos, unsigned plusNum);

	std::string insertFromCollectionToCollection(comb_hash_t combhash);
//	std::string insertToCollectionFromExtern(comb_hash_t combhash);
	std::string insertFromCollectionToPlaylist(comb_hash_t combhash);
	std::string insertFromPlaylist(comb_hash_t combhash);
	std::string insertFromNextlist(comb_hash_t combhash);

	std::string removeFromCollection(comb_hash_t combhash);
	std::string removeFromPlaylist(comb_hash_t combhash);
	std::string removeFromNextlist(comb_hash_t combhash);

	std::string dummy(comb_hash_t combhash);

	int addFunc(int a, int b);

};

#endif /* CDIFFBUILDER_H_ */
