/*
 * CMediaColUpdater.h
 *
 *  Created on: 30 Apr 2012
 *      Author: martin
 */

#ifndef CMEDIACOLUPDATER_H_
#define CMEDIACOLUPDATER_H_

#include <string>
#include <vector>

class CMediaItem;

class CMediaColUpdater {
public:
	CMediaColUpdater(std::string mediaColDir) throw(std::string);
	virtual ~CMediaColUpdater();

	int update(std::vector<CMediaItem*>* newCol);

private:
	std::string m_mediaColDir;
};

#endif /* CMEDIACOLUPDATER_H_ */
