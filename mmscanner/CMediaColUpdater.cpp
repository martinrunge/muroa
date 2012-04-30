/*
 * CMediaColUpdater.cpp
 *
 *  Created on: 30 Apr 2012
 *      Author: martin
 */

#include "CMediaColUpdater.h"

#include <CMediaItem.h>
#include <CRootItem.h>

using namespace std;

CMediaColUpdater::CMediaColUpdater(string mediaColDir)  throw(std::string) :
												        m_mediaColDir(mediaColDir){

}

CMediaColUpdater::~CMediaColUpdater() {
	// TODO Auto-generated destructor stub
}

int CMediaColUpdater::update(vector<CMediaItem*>* newCol) {

	return 0;
}
