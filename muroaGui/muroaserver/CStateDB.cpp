/*
 * CStateDB.cpp
 *
 *  Created on: 31 Jul 2010
 *      Author: martin
 */

#include "CStateDB.h"

#include <iostream>

using namespace std;

CStateDB::CStateDB(std::string dbFileName) : m_dbFileName( dbFileName ), m_db(0) {

}

CStateDB::~CStateDB() {
	close();
}


int CStateDB::open() {
	int rc = sqlite3_open(m_dbFileName.c_str(), &m_db);
	if( rc ){
	    cerr << "Can't open database: " << sqlite3_errmsg(m_db) << endl;
	    sqlite3_close(m_db);
	    m_db = 0;
    }
}

int CStateDB::close() {
	if(m_db != 0) {
		sqlite3_close(m_db);
	}
}


int CStateDB::callback(void* data, int numresult, char** columns, char** columnNames) {


	return 0;
}


