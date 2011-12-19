/*
 * CMsgOpenDb.h
 *
 *  Created on: 28 Nov 2010
 *      Author: martin
 */

#ifndef CMSGOPENDB_H_
#define CMSGOPENDB_H_

#include "CMsgBase.h"

#include <string>

class CMsgOpenDb: public CMsgBase {
public:
	CMsgOpenDb(std::string db_path);
	CMsgOpenDb(char* buffer, int size);
	virtual ~CMsgOpenDb();

	bool operator==(const CMsgOpenDb &other);

	std::string getDbPath() { return m_db_path; };

	char* serialize(int& size);

private:
	std::string m_db_path;


};

#endif /* CMSGOPENDB_H_ */
