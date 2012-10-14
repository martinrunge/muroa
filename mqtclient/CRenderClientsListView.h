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

class CMuroaListModel;
namespace muroa {
class CStreamClientItem;
}

class CRenderClientsListView : public QListView {
	Q_OBJECT;
public:
	CRenderClientsListView( QWidget * parent = 0 );
	virtual ~CRenderClientsListView();

	void setDiffBuilderPtr(CDiffBuilder* db) { m_diffBuilder = db; };

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

    QPoint m_startPos;
    bool m_dragActive;

    CDiffBuilder* m_diffBuilder;

    const QString dndMimeType;
};

#endif /* CRENDERCLIENTSLISTVIEW_H_ */
