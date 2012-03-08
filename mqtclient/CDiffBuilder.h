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

	typedef std::string (CDiffBuilder::*getItemPtr)(int);

	CDiffBuilder::getItemPtr getItemToRemove;
	CDiffBuilder::getItemPtr getItemToInsert;

//	std::string (CDiffBuilder::*getItemToRemove)(int);
//	std::string (CDiffBuilder::*getItemToInsert)(int);

	void prepareDiff(CModelDiff* md);
	std::string prepareDiffHeader(unsigned minusPos, unsigned minusNum, unsigned plusPos, unsigned plusNum);

	std::string insertFromCollectionToCollection(int pos);
//	std::string insertToCollectionFromExtern(int pos);
	std::string insertFromCollectionToPlaylist(int pos);
	std::string insertFromPlaylist(int pos);
	std::string insertFromNextlist(int pos);

	std::string removeFromCollection(int pos);
	std::string removeFromPlaylist(int pos);
	std::string removeFromNextlist(int pos);

	std::string dummy(int pos);

	int addFunc(int a, int b);

};

#endif /* CDIFFBUILDER_H_ */
