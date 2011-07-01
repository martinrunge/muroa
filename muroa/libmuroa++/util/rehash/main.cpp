/***************************************************************************
 *
 *   main.cpp
 *
 *   This file is part of libmuroa++                                  *
 *   Copyright (C) 2011 by Martin Runge <martin.runge@web.de>           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <sstream>
#include <fstream>
#include "CUtils.h"
#include "CRootItem.h"
#include "CMediaItem.h"
#include <cstring>

#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"


namespace fs = boost::filesystem;

using namespace std;

void walkTree(string dir);

int main(int argc, char* argv[]) {

	string pwd = fs::current_path().string();
	walkTree(pwd);

}

string rehashLine(string line) {
	int numTab = 0;

	int tabpos = 0;
	do {
		tabpos = line.find('\t', tabpos + 1);
		numTab++;

	}
	while(tabpos != string::npos);
	numTab--;

	switch(numTab) {
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	{
		// last block is hash
//		size_t lasTabPos = line.rfind('\t');
//		string hashStr = line.substr(lastTabPos + 1, line.size - lasTabPos - 1 );
//		string rest = line.substr(lastTabPos);
		string diffsign = "";
		string origline = line;
		if(line[0] == '-') {
			diffsign = "-";
			line = line.substr(1, line.size() - 1 );
		}
		else if(line[0] == '+') {
			diffsign = "+";
			line = line.substr(1, line.size() - 1 );
		}

		CRootItem ritem;
		IContentItem* contItem = ritem.addContentItem(line);
		CMediaItem *mitem = reinterpret_cast<CMediaItem*>(contItem);
		if(mitem == 0) {
			cerr << "Could not deserialize Item in file " << endl;
			return origline;
		}

		string retval = diffsign + mitem->serialize();

		cerr << origline << " -> " << retval << endl;
		return retval;
		break;
	}
	default:
		// keep line
		return line + "\n";
	}
}

int rehashFile(string filename) {
	cout << "->" << filename << endl;
	// return 0;

	string newfilename = filename + ".new";
    ifstream file(filename);
    ofstream ofile(newfilename);

    string line ;
    while( getline( file, line ) )
    {
		if( line.size() < 3)  continue;
		// assert( cline[0] != 0 && cline[1] != 0 && cline[2] != 0 );
		line = rehashLine(line);
		ofile << line;
    }

    fs::rename(filename, filename + ".orig" );
    fs::rename(newfilename, filename );
}


struct iterstate {
	fs::path path;
	fs::directory_iterator dirIter;
	int depth;
};

void walkTree(string dir) {

	fs::path full_path( dir );

	unsigned long file_count = 0;
	unsigned long dir_count = 0;
	unsigned long other_count = 0;
	unsigned long err_count = 0;

	unsigned long dir_depth_count = 0;

	if ( !fs::exists( full_path ) || !fs::is_directory( full_path ) )
	{
		std::cout << "\nNot found: " << full_path.file_string() << std::endl;
		return;
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
					//std::cerr << state.dirIter->path() << "  "  << state.dirIter->path().extension() << "\n";
					if( string(".txt").compare(state.dirIter->path().extension()) == 0 ) {
						rehashFile(state.dirIter->path().string());
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


//	std::cout << "\n" << file_count << " files\n"
//			<< dir_count << " directories\n"
//			<< other_count << " others\n"
//			<< err_count << " errors\n";

	return;
}

