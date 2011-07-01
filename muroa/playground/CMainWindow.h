/***************************************************************************
 *                                                                         *
 *   CMainWindow.h                                                         *
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

#ifndef MUROAGUI_H
#define MUROAGUI_H

#include <QtGui/QMainWindow>
#include <QLabel>
#include "CTreeModel.h"
#include "ui_MainWindow.h"

class CMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    CMainWindow(QWidget *parent = 0);
    ~CMainWindow();

public slots:
	void patch(void);

private:
    Ui::MainWindow ui;

    CTreeModel *m_leftTreeModel;
    CTreeModel *m_rightTreeModel;

    QLabel m_status_label;

};

#endif // MUROAGUI_H
