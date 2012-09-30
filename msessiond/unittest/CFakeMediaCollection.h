/*
 * CFakeMediaCollection.h
 *
 *  Created on: 21 Dec 2010
 *      Author: martin
 */

#ifndef CFAKEMEDIACOLLECTION_H_
#define CFAKEMEDIACOLLECTION_H_

#include <string>
#include <vector>

namespace muroa {
  class CMediaItem;
}

class CFakeMediaCollection {
public:
	CFakeMediaCollection(std::string indir);
	virtual ~CFakeMediaCollection();

	std::vector<muroa::CMediaItem*>* collectionWithoutFiles( int size );
	int mp3Ccollection(int numArtists, int numAlbumsPerArtist, int numTitlesPerAlbum);
private:

	void createMp3File(std::string filename);

	std::string m_rootdir;
};

#endif /* CFAKEMEDIACOLLECTION_H_ */
