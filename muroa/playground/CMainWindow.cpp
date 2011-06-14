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
    //connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(openConnection()));
    //connect(ui.actionClose, SIGNAL(triggered()), &m_connection, SLOT(close()));

	statusBar()->addWidget(&m_status_label);

	std::string exampleCollection = CUtils::file2string( "testcases/example.txt" );

	CRootItem rItem;
	rItem.deserialize(exampleCollection);

	m_leftTreeModel = new CTreeModel(&rItem);
	// m_rightTreeModel = new CTreeModel(rItem);

	ui.leftTreeView->setModel(m_leftTreeModel);
	// ui.rightTreeView->setModel(m_rightTreeModel);
}

CMainWindow::~CMainWindow()
{
	//delete m_rightTreeModel;
	delete m_leftTreeModel;
}

