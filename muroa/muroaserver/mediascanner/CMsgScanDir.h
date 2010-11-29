/*
 * CMsgScanDir.h
 *
 *  Created on: 1 Nov 2010
 *      Author: martin
 */

#ifndef CMSGSCANDIR_H_
#define CMSGSCANDIR_H_

#include "CMsgBase.h"

#include <string>

class CMsgScanDir: public CMsgBase {
public:
	CMsgScanDir(std::string path);
	CMsgScanDir(char* buffer, int size);
	virtual ~CMsgScanDir();

	bool operator==(const CMsgScanDir &other);

	std::string getPath() { return m_path; };

	char* serialize(int& size);

private:
	std::string m_path;
};

#endif /* CMSGSCANDIR_H_ */
