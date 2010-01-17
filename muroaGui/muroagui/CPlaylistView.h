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



class CPlaylistView : public QListView
{
	Q_OBJECT;
public:
	CPlaylistView(QWidget * parent = 0 );
	virtual ~CPlaylistView();

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


};

#endif /* CPLAYLISTVIEW_H_ */
