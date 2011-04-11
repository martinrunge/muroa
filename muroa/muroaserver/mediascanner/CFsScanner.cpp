/*
 * CFsScanner.cpp
 *
 *  Created on: 16 Jul 2010
 *      Author: martin
 */

#include "CFsScanner.h"
#include <iostream>
#include <stack>
#include <vector>
#include <queue>


#include <taglib/fileref.h>
#include <taglib/tag.h>

#include "CMediaItem.h"

#include "CMediaScanner.h"
#include "CMsgFinished.h"
#include "CMsgProgress.h"

using namespace std;

CFsScanner::CFsScanner(CMediaScanner* parent) : m_scanResult(0), m_scanning(false), m_parent( parent ), m_jobID(0) {
}

CFsScanner::~CFsScanner() {
	m_progressDirs.clear();
	if(m_thread.joinable()) {
		m_thread.join();
	}
}

void CFsScanner::scanDir(std::string dir, uint32_t jobID ) {
	if(m_scanning == false) {
		m_scanning = true;
		m_jobID = jobID;
		m_thread = thread(&CFsScanner::walkTree, this, dir );
	}
}

void CFsScanner::scanDirBFS(std::string dir) {
	if(m_scanning == false) {
		m_scanning = true;
		m_thread = thread(&CFsScanner::walkTreeBFS, this, dir );
	}
}


std::vector<CMediaItem*>* CFsScanner::finishScan() {
	if(m_scanning == false) {
		return 0;
	}
	else {

		m_thread.join();
		m_scanning = false;
		m_jobID = 0;
	}
	std::vector<CMediaItem*>* retval = m_scanResult;
	m_scanResult = 0;
	return retval;
}

int CFsScanner::walkTree(string dir) {
	walkTreeBFS( dir );

	fs::path full_path( dir );

	unsigned long file_count = 0;
	unsigned long dir_count = 0;
	unsigned long other_count = 0;
	unsigned long err_count = 0;

	unsigned long dir_depth_count = 0;

	cleanupScanResult();

	m_scanResult = new vector<CMediaItem*>;

	if ( !fs::exists( full_path ) || !fs::is_directory( full_path ) )
	{
		std::cout << "\nNot found: " << full_path.file_string() << std::endl;
		return -1;
	}
	// use a stack of struct iterstate instead of resursion;
	stack<struct iterstate> itstack;

	struct iterstate initState;

	initState.path = full_path;
	initState.dirIter = fs::directory_iterator( full_path );
	initState.depth = 0;
	itstack.push(initState);
	while(itstack.size() > 0) {
		struct iterstate state = itstack.top();
		itstack.pop();

		fs::directory_iterator end_iter;
		while( state.dirIter != end_iter)  {
			try {
				if ( fs::is_directory( state.dirIter->status() ) ) {
					++dir_count;

					if( itstack.size() <= m_progress_depth ) {
						int old_progress = m_progress;
						dir_depth_count++;
						m_progress = 100.0 * (float)dir_depth_count / m_progress_num_dirs;
						// cerr << "progress: " << m_progress << " " << state.path << "itstack.size(): " << itstack.size()  << endl;
						if(m_progress != old_progress) {
							CMsgProgress *progressMsg = new CMsgProgress(m_jobID, m_progress);
							m_parent->postEvent(progressMsg);
						}
					}

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
						CMediaItem* item = readTag(state.dirIter->path());
						if(item != 0) {
							m_scanResult->push_back(item);
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

	CMsgProgress *progressMsg = new CMsgProgress(m_jobID, 100);
	m_parent->postEvent(progressMsg);

	CMsgFinished* finiMsg = new CMsgFinished(m_jobID);
	m_parent->postEvent(finiMsg);

	std::cout << "\n" << file_count << " files\n"
			<< dir_count << " directories\n"
			<< other_count << " others\n"
			<< err_count << " errors\n";

	return 0;
}

int CFsScanner::walkTreeBFS(std::string dir) {
	fs::path full_path( fs::system_complete(dir) );

	unsigned long file_count = 0;
	unsigned long dir_count = 0;
	unsigned long other_count = 0;
	unsigned long err_count = 0;
	unsigned long depth = 0;

	cleanupScanResult();
	m_progressDirs.clear();

	m_scanResult = new vector<CMediaItem*>;

	if ( !fs::exists( full_path ) || !fs::is_directory( full_path ) )
	{
		std::cout << "\nNot found: " << full_path.file_string() << std::endl;
		return -1;
	}
	// use a stack of struct iterstate instead of resursion;
	queue<struct iterstate> itqueue;

	struct iterstate initState;

	initState.path = full_path;
	initState.depth = 0;
	depth = initState.depth;
	//initState.dirIter = fs::directory_iterator( full_path );
	itqueue.push(initState);
	while(itqueue.size() > 0) {

		struct iterstate state = itqueue.front();
		itqueue.pop();

		if( state.depth > depth ) {
			if( dir_count > 100) {
				m_progress_depth = depth;
				m_progress_num_dirs = dir_count;
				break;
			}
			else {
				depth = state.depth;
			}
		}

		state.dirIter = fs::directory_iterator( state.path );
		// std::cerr << "\nIn directory: " << state.path.directory_string() << "\n";

		fs::directory_iterator end_iter;
		while( state.dirIter != end_iter)  {
			try {
				// cerr << state.dirIter->path() << endl;

				if ( fs::is_directory( state.dirIter->status() ) ) {
					++dir_count;
					std::cerr << state.dirIter->path() << " [directory]\n";
					struct iterstate newState;
					newState.path = state.dirIter->path();
					newState.depth = state.depth + 1;

					m_progressDirs.push_back( state.dirIter->path().string() );

					itqueue.push(newState);
//					cerr << newState.path << endl;
				}
			}
			catch ( const std::exception & ex ) {
				++err_count;
				std::cout << state.dirIter->path().filename() << " " << ex.what() << std::endl;
			}
			state.dirIter++;
		}
	}
	m_progress_dirs_size = m_progressDirs.size();
	m_progress_num_dirs = m_progressDirs.size();


	std::cout << "\n" << file_count << " files\n"
			<< dir_count << " directories\n"
			<< other_count << " others\n"
			<< err_count << " errors\n"
			<< depth << " max depth\n";

	return 0;
}


void CFsScanner::cleanupScanResult() {
	if(m_scanResult != 0) {
		for(std::vector<CMediaItem*>::iterator it = m_scanResult->begin(); it != m_scanResult->end(); it++ ) {
			CMediaItem* item = *it;
			delete item;
		}
		delete m_scanResult;
		m_scanResult = 0;
	}
}


bool CFsScanner::knownType( fs::path path ) {
	string extension = path.extension();
	for( vector<string>::const_iterator it = m_types.begin(); it != m_types.end(); it++ ) {
		if( it->compare(extension) == 0 ) return true;
	}
	return false;
}

CMediaItem* CFsScanner::readTag( fs::path path ) {
	// cout << "add: " << path;

	CMediaItem* item = new CMediaItem();

	TagLib::FileRef f( path.string().c_str() );
	if( !f.isNull()) {
		TagLib::String artist = f.tag()->artist();
		TagLib::String album  = f.tag()->album();
		TagLib::String title = f.tag()->title();

		// cout << " Artist: " << artist.to8Bit(true) << " Album: " << album.to8Bit(true) << " Title; " << title.to8Bit(true) << endl;

		item->setFilename( path.string() );
		item->setAlbum( album.to8Bit(true) );
		item->setArtist( artist.to8Bit(true) );
		item->setTitle( title.to8Bit(true) );
		item->setYear( f.tag()->year() );
	}
	// m_decoder.open(path.string().c_str());
	// int duration = m_decoder.getDuration();
	// m_decoder.close();
	// item->setDuration(duration);
	return item;
}
