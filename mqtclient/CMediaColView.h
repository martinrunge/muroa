/*
 * CCollectionView.h
 *
 *  Created on: 23 Jan 2010
 *      Author: martin
 */

#ifndef CMEDIACOLVIEW_H_
#define CMEDIACOLVIEW_H_

#include <QTreeView>

#include "cmds/CmdBase.h"
#include "cmds/CmdEditPlaylist.h"

class QMouseEvent;
class QDragEnterEvent;
class QDragMoveEvent;
class QDropEvent;
class CDiffBuilder;


class CMediaColView : public QTreeView {
public:
	CMediaColView(QWidget * parent = 0);
	virtual ~CMediaColView();

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

#endif /* CMEDIACOLVIEW_H_ */
