/*
 * CRenderClientsListView.cpp
 *
 *  Created on: 1 Oct 2012
 *      Author: martin
 */

#include <QtGui>
#include <QDebug>
#include <QMimeData>

#include "CRenderClientsListView.h"
#include "CStreamClientItem.h"
#include "CRenderClientsListModel.h"

#include "CRenderClientsDiffBuilder.h"

using namespace muroa;


CRenderClientsListView::CRenderClientsListView(QWidget * parent ) : QListView( parent ),
                                                  m_dragActive(false),
                                                  m_rcDiffBuilder(0),
                                                  dndMimeType("application/x-muroa-render-clients") {
	setAcceptDrops(true);
	viewport()->setAcceptDrops(true);
	setDragDropMode(QAbstractItemView::DragDrop);
	setDropIndicatorShown(true);
	setSelectionMode(QAbstractItemView::ExtendedSelection);
	//setDragDropMode( QAbstractItemView::DragDrop );
	setIconSize(QSize(64, 64));
}

CRenderClientsListView::~CRenderClientsListView() {
}

void CRenderClientsListView::mousePressEvent(QMouseEvent *event)
{
	qDebug() << QString("mousePressEvent");
    if (event->button() == Qt::LeftButton && !m_dragActive)
        m_startPos = event->pos();
    QListView::mousePressEvent(event);
}

void CRenderClientsListView::mouseMoveEvent(QMouseEvent *event)
{
	qDebug() << QString("mouseMoveEvent");
    if (event->buttons() & Qt::LeftButton && !m_dragActive) {
        int distance = (event->pos() - m_startPos).manhattanLength();
        if (distance >= QApplication::startDragDistance())
            performDrag();
    }
    //QListView::mouseMoveEvent(event);
}

void CRenderClientsListView::dragEnterEvent(QDragEnterEvent *event)
{
	qDebug() << QString("dragEnterEvent");
    //CRenderClientsListView *source = qobject_cast<CRenderClientsListView *>(event->source());
	const QMimeData* data = event->mimeData();

    if (data && data->hasFormat(dndMimeType)) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void CRenderClientsListView::dragMoveEvent(QDragMoveEvent *event)
{
	qDebug() << QString("dragMoveEvent");
    // CRenderClientsListView *source = qobject_cast<CRenderClientsListView *>(event->source());
	const QMimeData* data = event->mimeData();

    if (data && data->hasFormat(dndMimeType)) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void CRenderClientsListView::dropEvent(QDropEvent *event)
{
	qDebug() << QString("dropEvent");
	const QMimeData* data = event->mimeData();

    if (data && data->hasFormat(dndMimeType)) {
        event->setDropAction(Qt::MoveAction);
        event->accept();

        // CPlaylistModel* plModel = reinterpret_cast<CPlaylistModel*>(model());

        CModelDiff md(data->data(dndMimeType));
        md.setCommandType(E_OWN_RENDER_CLIENT);
        QModelIndex currentIdx = indexAt( event->pos());
        //md.appendToInsert(currentIdx.row(), plModel->itemAt(currentIdx.row())->getHash());
        int insertPos = currentIdx.row();
        if(insertPos == -1)
        {
        	insertPos = model()->rowCount();
        }
        md.setInsertPos( insertPos );
        md.setDestination(m_role);

        qDebug() << QString("CRenderClientsListView::dropEvent: Move [%1,%2] to %3").arg(md.getSelectedItems().at(0).line).arg(md.getSelectedItems().at(md.getNumSelected() - 1 ).line).arg(md.getInsertPos());
        // plModel->makeDiff(&md);
        m_rcDiffBuilder->diff(md);
    }
}

void CRenderClientsListView::performDrag()
{
	qDebug() << QString("performDrag");

    CRenderClientsListModel* lmodel = reinterpret_cast<CRenderClientsListModel*>(model());

    QModelIndexList indexList = selectedIndexes();
    CModelDiff md(m_role);

    for(int i = 0; i < indexList.size(); i++)
    {
    	comb_hash_t combhash;
		CItemBase* item = lmodel->itemFromIndex(indexList.at(i));
		switch(item->type()) {
		case CItemType::E_STREAM_CLIENT:
		{
    		CStreamClientItem* scItem = reinterpret_cast<CStreamClientItem*>(item);
			combhash.type = scItem->type();
			if(m_role == E_OWN_RENDER_CLIENT) {
			    int filteredline = lmodel->own2availIndex(indexList.at(i).row());
	            combhash.line = filteredline;
			}
			else {
                combhash.line = indexList.at(i).row();
			}

			md.appendToSelectedItems(combhash);
		}
		default:
			break;
		}
    }

    if (md.getNumSelected() > 0)
    {
    	QMimeData *mimeData = new QMimeData;
        //mimeData->setText(item->asString());
        mimeData->setData(dndMimeType, md.toQByteArray());

        m_dragActive = true;
        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        // drag->setPixmap(QPixmap(":/images/person.png"));

        Qt::DropAction action = drag->exec(Qt::MoveAction);

        m_dragActive = false;
        qDebug() << QString("DropAction: %1").arg(action);

    }
}
