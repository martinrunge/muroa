/*
 * CDiffBuilder.h
 *
 *  Created on: 16 Feb 2010
 *      Author: martin
 */

#ifndef CDIFFBUILDER_H_
#define CDIFFBUILDER_H_

#include "CCollection.h"
#include "CModelDiff.h"

class CCollectionItem;
class CPlaylistItem;

class CDiffBuilder {
public:
	CDiffBuilder(CCollection<CCollectionItem*>* colPtr, CCollection<CPlaylistItem*>* plPtr, CCollection<CPlaylistItem*>* m_pnPtr );
	virtual ~CDiffBuilder();

	QString diff(const CModelDiff& md);

private:
	CCollection<CCollectionItem*>* m_collectionPtr;
	CCollection<CPlaylistItem*>* m_playlistPtr;
	CCollection<CPlaylistItem*>* m_playnextPtr;

};

#endif /* CDIFFBUILDER_H_ */
