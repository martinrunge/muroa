/*
 * CCollectionUpdater.cpp
 *
 *  Created on: 16 Jul 2010
 *      Author: martin
 */

#include "CCollectionUpdater.h"
#include <iostream>
#include <stack>

#include <taglib/fileref.h>
#include <taglib/tag.h>

#include "../CCollectionItem.h"

using namespace std;

CCollectionUpdater::CCollectionUpdater() {
	// TODO Auto-generated constructor stub

}

CCollectionUpdater::~CCollectionUpdater() {
	// TODO Auto-generated destructor stub
}

CCollection<CCollectionItem>* CCollectionUpdater::walkTree(string dir) {

	fs::path full_path( dir );

	unsigned long file_count = 0;
	unsigned long dir_count = 0;
	unsigned long other_count = 0;
	unsigned long err_count = 0;

	CCollection<CCollectionItem>* collection = new CCollection<CCollectionItem>;

	if ( !fs::exists( full_path ) || !fs::is_directory( full_path ) )
	{
		std::cout << "\nNot found: " << full_path.file_string() << std::endl;
		return collection;
	}
	// use a stack of struct iterstate instead of resursion;
	stack<struct iterstate> itstack;

	struct iterstate initState;

	initState.path = full_path;
	initState.dirIter = fs::directory_iterator( full_path );
	itstack.push(initState);
	while(itstack.size() > 0) {
		struct iterstate state = itstack.top();
		itstack.pop();

		// std::cerr << "\nIn directory: " << state.path.directory_string() << "\n";

		fs::directory_iterator end_iter;
		while( state.dirIter != end_iter)  {
			try {
				if ( fs::is_directory( state.dirIter->status() ) ) {
					++dir_count;
					// std::cerr << state.dirIter->path() << " [directory]\n";
					struct iterstate newState;
					newState.path = state.dirIter->path();
					newState.dirIter = fs::directory_iterator( state.dirIter->path() );

					state.dirIter++;      // proceed one entry so we do not descend into this subdir when the state gets poped later on.
					itstack.push(state);

					state = newState;
					continue;
				}
				else if ( fs::is_regular_file( state.dirIter->status() ) ) {
					++file_count;
					// std::cerr << state.dirIter->path() << "  "  << state.dirIter->path().extension() << "\n";
					if( knownType( state.dirIter->path() ) ) {
						CCollectionItem* item = readTag(state.dirIter->path());
						if(item != 0) {
							collection->insert(item);
						}
					}
				}
				else {
					++other_count;
					// std::cerr << state.dirIter->path() << " [other]\n";
				}

			}
			catch ( const std::exception & ex ) {
				++err_count;
				std::cout << state.dirIter->path().filename() << " " << ex.what() << std::endl;
			}
			state.dirIter++;
		}
	}

	std::cout << "\n" << file_count << " files\n"
			<< dir_count << " directories\n"
			<< other_count << " others\n"
			<< err_count << " errors\n";
	return collection;
}


bool CCollectionUpdater::knownType( fs::path path ) {
	string extension = path.extension();
	for( vector<string>::const_iterator it = m_types.begin(); it != m_types.end(); it++ ) {
		if( it->compare(extension) == 0 ) return true;
	}
	return false;
}

CCollectionItem* CCollectionUpdater::readTag( fs::path path ) {
	// cout << "add: " << path;

	TagLib::FileRef f( path.string().c_str() );
	TagLib::String artist = f.tag()->artist();
	TagLib::String album  = f.tag()->album();
	TagLib::String title = f.tag()->title();

	// cout << " Artist: " << artist.to8Bit(true) << " Album: " << album.to8Bit(true) << " Title; " << title.to8Bit(true) << endl;

	CCollectionItem* item = new CCollectionItem();
	item->setFilename( path.string() );
	item->setAlbum( album.to8Bit(true) );
	item->setArtist( artist.to8Bit(true) );
	item->setTitle( title.to8Bit(true) );
	item->setYear( f.tag()->year() );

	m_decoder.open(path.string().c_str());
	int duration = m_decoder.getDuration();
	m_decoder.close();
	item->setDuration(duration);
	return item;
}
