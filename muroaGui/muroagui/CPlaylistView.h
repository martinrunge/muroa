/*
 * CPlaylistView.h
 *
 *  Created on: 6 Jan 2010
 *      Author: martin
 */

#ifndef CPLAYLISTVIEW_H_
#define CPLAYLISTVIEW_H_

#include <QListView>

#include "CCommandBase.h"
#include "CPlaylistCommand.h"

class QMouseEvent;
class QDragEnterEvent;
class QDragMoveEvent;
class QDropEvent;
class CDiffBuilder;

class CPlaylistView : public QListView
{
	Q_OBJECT;
public:
	CPlaylistView(QWidget * parent = 0 );
	virtual ~CPlaylistView();

	void setDiffBuilderPtr(CDiffBuilder* db) { m_diffBuilder = db; };

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

private:
    void performDrag();

    QPoint m_startPos;
    bool m_dragActive;

    CDiffBuilder* m_diffBuilder;
};

#endif /* CPLAYLISTVIEW_H_ */
