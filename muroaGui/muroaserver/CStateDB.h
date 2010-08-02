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

    /** Revision table is very simple: rev_id , rev_nr
     *  There is a revision table for the collection, playlist and nextlist.
     */
    void createRevisionsTable(std::string name);

    void createCollectionTable(std::string name);
};

#endif /* CSTATEDB_H_ */
