/*
 * CDiffBuilder.h
 *
 *  Created on: 16 Feb 2010
 *      Author: martin
 */

#ifndef CRENDERCLIENTSDIFFBUILDER_H_
#define CRENDERCLIENTSDIFFBUILDER_H_

#include <QObject>
#include <QDebug>

#include <CRootItem.h>
#include "CModelDiff.h"
#include "cmds/CmdBase.h"


class CSession;

namespace muroa {
  class CStreamClientItem;
}


class CRenderClientsDiffBuilder : public QObject {
	Q_OBJECT;

public:
	CRenderClientsDiffBuilder( muroa::CRootItem* sstPtr, const CSession* const session );
	virtual ~CRenderClientsDiffBuilder();

	std::string diff(CModelDiff md);

signals:
	void sendCommand(CmdBase* cmd);


private:
	muroa::CRootItem* m_sessionStatePtr;
	const CSession* const m_session;

	muroa::CCategoryItem* m_render_clients;

	typedef std::string (CRenderClientsDiffBuilder::*getItemPtr)(comb_hash_t);

	CRenderClientsDiffBuilder::getItemPtr getItemToRemove;
	CRenderClientsDiffBuilder::getItemPtr getItemToInsert;

	std::string insertInOwnList(comb_hash_t comb_hash);
	std::string insertInAvailList(comb_hash_t comb_hash);
	std::string removeFromOwnList(comb_hash_t comb_hash);
	std::string removeFromAvailList(comb_hash_t comb_hash);

	void prepareDiff(CModelDiff* md);
	std::string prepareDiffHeader(unsigned minusPos, unsigned minusNum, unsigned plusPos, unsigned plusNum);

	std::string dummy(comb_hash_t combhash);

	int addFunc(int a, int b);
};

#endif /* CRENDERCLIENTSDIFFBUILDER_H_ */
