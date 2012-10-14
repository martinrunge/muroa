/*
 * CRenderClientsListView.h
 *
 *  Created on: 1 Oct 2012
 *      Author: martin
 */

#ifndef CRENDERCLIENTSLISTVIEW_H_
#define CRENDERCLIENTSLISTVIEW_H_

#include <QListView>

#include "cmds/CmdBase.h"
#include "cmds/CmdEditPlaylist.h"
#include "CModelDiff.h"

class QMouseEvent;
class QDragEnterEvent;
class QDragMoveEvent;
class QDropEvent;
class CDiffBuilder;

class CRenderClientsListView : public QListView {
	Q_OBJECT;
public:
	CRenderClientsListView( QWidget * parent = 0 );
	virtual ~CRenderClientsListView();

	void setDiffBuilderPtr(CDiffBuilder* db) { m_diffBuilder = db; };
	void setRole(enum origin role ) { m_role = role; };
	enum origin getRole() { return m_role; };

	inline void setDiffBuilder(CDiffBuilder* diffBuilderPtr) {m_diffBuilder = diffBuilderPtr; };
	inline CDiffBuilder* getDiffBuilder(CDiffBuilder* diffBuilderPtr) {return m_diffBuilder; };

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

private:
    void performDrag();

    enum origin m_role;

    QPoint m_startPos;
    bool m_dragActive;

    CDiffBuilder* m_diffBuilder;
};

#endif /* CRENDERCLIENTSLISTVIEW_H_ */
