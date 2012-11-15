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
#include "cmds/CmdEditSessionState.h"
#include "CModelDiff.h"

class QMouseEvent;
class QDragEnterEvent;
class QDragMoveEvent;
class QDropEvent;
class CRenderClientsDiffBuilder;

class CMuroaListModel;
namespace muroa {
class CStreamClientItem;
}

class CRenderClientsListView : public QListView {
	Q_OBJECT;
public:
	CRenderClientsListView( QWidget * parent = 0 );
	virtual ~CRenderClientsListView();

	inline void setDiffBuilder(CRenderClientsDiffBuilder* diffBuilderPtr) {m_rcDiffBuilder = diffBuilderPtr; };
	inline CRenderClientsDiffBuilder* getDiffBuilder(void) {
		return m_rcDiffBuilder;
	}

	enum origin getRole() const {
		return m_role;
	}

	void setRole(enum origin role) {
		m_role = role;
	}

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

    CRenderClientsDiffBuilder* m_rcDiffBuilder;

    enum origin m_role;
    const QString dndMimeType;
};

#endif /* CRENDERCLIENTSLISTVIEW_H_ */
