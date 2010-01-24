/*
 * CCollectionView.h
 *
 *  Created on: 23 Jan 2010
 *      Author: martin
 */

#ifndef CCOLLECTIONVIEW_H_
#define CCOLLECTIONVIEW_H_

#include <QTreeView>

#include "CCommandBase.h"
#include "CPlaylistCommand.h"

class QMouseEvent;
class QDragEnterEvent;
class QDragMoveEvent;
class QDropEvent;


class CCollectionView : public QTreeView {
public:
	CCollectionView(QWidget * parent = 0);
	virtual ~CCollectionView();
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

#endif /* CCOLLECTIONVIEW_H_ */
