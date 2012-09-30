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

namespace muroa {
  class CCollectionItem;
  class CPlaylistItem;
}


class CDiffBuilder : public QObject {
	Q_OBJECT;

public:
	CDiffBuilder(muroa::CRootItem* mediaColPtr, muroa::CRootItem* plPtr, muroa::CRootItem* nlPtr );
	virtual ~CDiffBuilder();

	std::string diff(CModelDiff md);

signals:
	void sendCommand(CmdBase* cmd);


private:
	muroa::CRootItem* m_mediaColPtr;
	muroa::CRootItem* m_plPtr;
	muroa::CRootItem* m_nlPtr;

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
	std::string insertFromCollectionToNextlist(comb_hash_t combhash);
	std::string insertFromPlaylistToPlaylist(comb_hash_t combhash);
	std::string insertFromPlaylistToNextlist(comb_hash_t combhash);
	std::string insertFromNextlistToNextlist(comb_hash_t combhash);

	std::string removeFromCollection(comb_hash_t combhash);
	std::string removeFromPlaylist(comb_hash_t combhash);
	std::string removeFromNextlist(comb_hash_t combhash);

	std::string dummy(comb_hash_t combhash);

	int addFunc(int a, int b);

};

#endif /* CDIFFBUILDER_H_ */
