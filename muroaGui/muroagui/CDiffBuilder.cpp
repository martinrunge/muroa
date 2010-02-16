/*
 * CDiffBuilder.cpp
 *
 *  Created on: 16 Feb 2010
 *      Author: martin
 */

#include "CDiffBuilder.h"

CDiffBuilder::CDiffBuilder(CCollection<CCollectionItem*>* colPtr,
						   CCollection<CPlaylistItem*>* plPtr,
						   CCollection<CPlaylistItem*>* pnPtr ):
	m_collectionPtr(colPtr) ,
	m_playlistPtr(plPtr) ,
	m_playnextPtr(pnPtr)
{

}

CDiffBuilder::~CDiffBuilder() {

}

QString CDiffBuilder::diff(const CModelDiff& md)
{
	QString ret;



	return ret;
}
