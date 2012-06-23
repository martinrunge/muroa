/*
 * CPlaylistIdProvider.cpp
 *
 *  Created on: 23 Jun 2012
 *      Author: martin
 */

#include "CPlaylistIdProvider.h"

#include "CSession.h"
#include "CApp.h"

CPlaylistIdProvider::CPlaylistIdProvider(CSession* session) : m_session(session)
{
}

CPlaylistIdProvider::~CPlaylistIdProvider()
{
}

uint32_t CPlaylistIdProvider::getNextFreeID()
{
}

void CPlaylistIdProvider::setNextFreeID(uint32_t id)
{
}

