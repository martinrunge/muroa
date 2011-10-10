/***************************************************************************
 *                                                                         *
 *   CMainWindow.cpp                                                       *
 *                                                                         *
 *   This file is part of libmuroa++                                       *
 *   Copyright (C) 2011 by Martin Runge <martin.runge@web.de>              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "CMainWindow.h"

#include <string>

#include "CUtils.h"
#include "CRootItem.h"


CMainWindow::CMainWindow(QWidget *parent)
    : QMainWindow(parent) {

	ui.setupUi(this);


	connect(ui.action_Exit, SIGNAL(triggered()), qApp, SLOT(quit()));
	connect(ui.actionPatch_collection, SIGNAL(triggered()), this, SLOT(patchCollection()));
	connect(ui.actionPatch_playlist, SIGNAL(triggered()), this, SLOT(patchPlaylist()));
	connect(ui.actionPatch_nextlist, SIGNAL(triggered()), this, SLOT(patchNextlist()));
    //connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(openConnection()));
    //connect(ui.actionClose, SIGNAL(triggered()), &m_connection, SLOT(close()));

	statusBar()->addWidget(&m_status_label);

	std::string exampleCollection = CUtils::file2string( "testcases/example.txt" );
	std::string modifiedCollection = CUtils::file2string( "testcases/modified.txt" );


	m_leftTreeModel = new CTreeModel();
	m_leftTreeModel->deserialize(exampleCollection);

	m_rightTreeModel = new CTreeModel();
	m_rightTreeModel->deserialize(modifiedCollection);

	ui.leftTreeView->setModel(m_leftTreeModel);
	ui.rightTreeView->setModel(m_rightTreeModel);


	std::string examplePlaylist = CUtils::file2string( "testcases/playlist.txt" );

	m_leftPlaylistModel = new CListModel();
	m_leftPlaylistModel->deserialize(examplePlaylist);
	m_leftPlaylistModel->setBase( m_leftPlaylistModel->getItemPtr("/Playlist 1"));

	m_rightPlaylistModel = new CListModel();
	m_rightPlaylistModel->deserialize(examplePlaylist);

	ui.leftPlaylistView->setModel(m_leftPlaylistModel);
	ui.rightPlaylistView->setModel(m_rightPlaylistModel);


//	m_leftNextlistModel = new CListModel();
//	m_leftNextlistModel->deserialize(exampleCollection);
//
//	m_rightNextlistModel = new CListModel();
//	m_rightNextlistModel->deserialize(modifiedCollection);
//
//	ui.leftNextlistView->setModel(m_leftNextlistModel);
//	ui.rightNextlistView->setModel(m_rightNextlistModel);
//


}

CMainWindow::~CMainWindow()
{
	//delete m_rightTreeModel;
	delete m_leftTreeModel;
	delete m_rightTreeModel;
}

void CMainWindow::patchCollection(void) {
	std::string diff = m_leftTreeModel->diff( *m_rightTreeModel );
	m_leftTreeModel->patch(diff);
}

void CMainWindow::patchPlaylist(void) {

}

void CMainWindow::patchNextlist(void) {

}
