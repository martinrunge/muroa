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

#include "boost/filesystem/path.hpp"

class CMediaItem;

class CMediaColUpdater {
public:
	CMediaColUpdater(std::string mediaColDir) throw(std::string);
	virtual ~CMediaColUpdater();

	int update(std::vector<CMediaItem*>* newCol);
	boost::filesystem::path getMediaColPath() { return m_mediaColPath; };

private:
	std::string m_mediaColDir;
	boost::filesystem::path m_mediaColPath;

	static const std::string mcrev_file_extension;

};

#endif /* CMEDIACOLUPDATER_H_ */
