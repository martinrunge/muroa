/*
 * CCollectionCommand.h
 *
 *  Created on: 10 Mar 2010
 *      Author: martin
 */

#ifndef CMD_EDIT_MEDIA_COL_H_
#define CMD_EDIT_MEDIA_COL_H_

#include "CmdBase.h"

class CmdEditMediaCol: public CmdBase {
public:
	CmdEditMediaCol( unsigned knownRev, QObject * parent = 0 );
	CmdEditMediaCol( unsigned knownRev, const std::string& diff, QObject * parent = 0 );
	virtual ~CmdEditMediaCol();

	void setData(const std::string& diff);

	void timeout();

};

#endif /* CMD_EDIT_MEDIA_COL_H_ */
