/*
 * CDiffBuilder.h
 *
 *  Created on: 16 Feb 2010
 *      Author: martin
 */

#ifndef CDIFFBUILDER_H_
#define CDIFFBUILDER_H_

#include <QObject>

#include "CCollection.h"
#include "CModelDiff.h"
#include "CCommandBase.h"

class CCollectionItem;
class CPlaylistItem;


class CDiffBuilder : public QObject {
	Q_OBJECT;

public:
	CDiffBuilder(CCollection<CCollectionItem*>* colPtr, CCollection<CPlaylistItem*>* plPtr, CCollection<CPlaylistItem*>* m_nlPtr );
	virtual ~CDiffBuilder();

	QString diff(CModelDiff md);

signals:
	void sendCommand(const CCommandBase& cmd);


private:
	CCollection<CCollectionItem*>* m_collectionPtr;
	CCollection<CPlaylistItem*>* m_playlistPtr;
	CCollection<CPlaylistItem*>* m_nextlistPtr;

	typedef QString (CDiffBuilder::*getItemPtr)(int);

	CDiffBuilder::getItemPtr getItemToRemove;
	CDiffBuilder::getItemPtr getItemToInsert;

//	QString (CDiffBuilder::*getItemToRemove)(int);
//	QString (CDiffBuilder::*getItemToInsert)(int);

	void prepareDiff(CModelDiff* md);


	QString insertFromCollectionToCollection(int pos);
//	QString insertToCollectionFromExtern(int pos);
	QString insertFromCollectionToPlaylist(int pos);
	QString insertFromPlaylist(int pos);
	QString insertFromNextlist(int pos);

	QString removeFromCollection(int pos);
	QString removeFromPlaylist(int pos);
	QString removeFromNextlist(int pos);

	QString dummy(int pos);

	int addFunc(int a, int b);

};

#endif /* CDIFFBUILDER_H_ */
