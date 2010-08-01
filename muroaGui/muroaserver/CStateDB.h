/*
 * CStateDB.h
 *
 *  Created on: 31 Jul 2010
 *      Author: martin
 */

#ifndef CSTATEDB_H_
#define CSTATEDB_H_

#include <string>
#include <sqlite3.h>

class CStateDB {
public:
	CStateDB(std::string dbFileName);
	virtual ~CStateDB();

	int open();
	int close();

	int callback(void* data, int numresult, char** columns, char** columnNames);


private:
	std::string m_dbFileName;
    sqlite3 *m_db;

    void createRevisionsTable();
    void createCollectionsTable();

};

#endif /* CSTATEDB_H_ */
