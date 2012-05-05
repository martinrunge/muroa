/*
 * CMediaColUpdater.cpp
 *
 *  Created on: 30 Apr 2012
 *      Author: martin
 */

#include "CMediaColUpdater.h"
#include "CMediaItemCategorizer.h"

#include <CMediaItem.h>
#include <CRootItem.h>
#include <CUtils.h>

#include "boost/filesystem/operations.hpp"

using namespace std;
using namespace boost::filesystem;
using namespace muroa;

const string CMediaColUpdater::mcrev_file_extension(".mcrev");


CMediaColUpdater::CMediaColUpdater(string mediaColDir)  throw(std::string) :
												        m_mediaColDir(mediaColDir),
												        m_mediaColPath(mediaColDir) {
	if(!exists(m_mediaColPath)) {
		throw string("no such directory: " + mediaColDir);
	}
}

CMediaColUpdater::~CMediaColUpdater() {
}

int CMediaColUpdater::update(vector<CMediaItem*>* newCol) {
	CRootItem* ri;
	ri = CMediaItemCategorizer::categorize(newCol);

	unsigned maxRev = 0;
	directory_iterator dir_it = directory_iterator( m_mediaColPath );
	while( dir_it != directory_iterator() )
	{
		string ext = dir_it->path().extension().string();
		if( ext.compare( mcrev_file_extension ) == 0 )
		{
			try {
				string revNrStr = dir_it->path().stem().string();
				unsigned long revNr = CUtils::str2uint32(revNrStr);
				maxRev = (revNr > maxRev) ? revNr : maxRev;
			}
			catch(invalid_argument& ex) {

			}
		}
		dir_it++;
	}
	ostringstream maxRevFnOss;
	maxRevFnOss << maxRev << mcrev_file_extension;
	CRootItem* maxRevRi = new CRootItem();
	maxRevRi->fromFile( maxRevFnOss.str() );

	unsigned newRev = maxRev + 1;
	ri->setRevision(maxRev);
	if(*ri == *maxRevRi ) {
		// scan result and max know revision are equal -> do nothing
		delete maxRevRi;
		delete ri;
		return 0;
	}

	ri->setRevision(newRev);

	ostringstream oss;
	oss << newRev << mcrev_file_extension;
	ri->deserialize(oss.str());
	delete maxRevRi;
	delete ri;

	return newRev;
}
