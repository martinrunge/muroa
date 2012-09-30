/*
 * CCollectionView.cpp
 *
 *  Created on: 23 Jan 2010
 *      Author: martin
 */
#include <QtGui>
#include <QDebug>
#include <QMimeData>

#include "CMediaColView.h"
#include "CMuroaListModel.h"
#include "CMuroaTreeModel.h"
#include <CMediaItem.h>
#include <CItemType.h>

#include "CModelDiff.h"
#include "CDiffBuilder.h"

using namespace muroa;

CMediaColView::CMediaColView(QWidget * parent) : QTreeView(parent){
	setAcceptDrops(true);
	viewport()->setAcceptDrops(true);
	setDragDropMode(QAbstractItemView::DragDrop);
	setDropIndicatorShown(true);
	setSelectionMode(QAbstractItemView::ExtendedSelection);
}

CMediaColView::~CMediaColView() {
	// TODO Auto-generated destructor stub
}

void CMediaColView::mousePressEvent(QMouseEvent *event)
{
	qDebug() << QString("mousePressEvent");
    if (event->button() == Qt::LeftButton && !m_dragActive)
        m_startPos = event->pos();
    QTreeView::mousePressEvent(event);
}

void CMediaColView::mouseMoveEvent(QMouseEvent *event)
{
	qDebug() << QString("mouseMoveEvent");
    if (event->buttons() & Qt::LeftButton && !m_dragActive) {
        int distance = (event->pos() - m_startPos).manhattanLength();
        if (distance >= QApplication::startDragDistance())
            performDrag();
    }
    //QTreeView::mouseMoveEvent(event);
}

void CMediaColView::dragEnterEvent(QDragEnterEvent *event)
{
	qDebug() << QString("dragEnterEvent");
    //CPlaylistView *source = qobject_cast<CPlaylistView *>(event->source());
	const QMimeData* data = event->mimeData();

    if (data && data->hasFormat("application/x-muroa-playlist-diff")) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void CMediaColView::dragMoveEvent(QDragMoveEvent *event)
{
	qDebug() << QString("dragMoveEvent");
    // CPlaylistView *source = qobject_cast<CPlaylistView *>(event->source());
	const QMimeData* data = event->mimeData();

    if (data && data->hasFormat("application/x-muroa-playlist-diff")) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void CMediaColView::dropEvent(QDropEvent *event)
{
	qDebug() << QString("dropEvent");
	const QMimeData* data = event->mimeData();

    if (data && data->hasFormat("application/x-muroa-playlist-diff")) {
        event->setDropAction(Qt::MoveAction);
        event->accept();

        CMuroaListModel* plModel = reinterpret_cast<CMuroaListModel*>(model());

        CModelDiff md(data->data("application/x-muroa-playlist-diff"));
        QModelIndex currentIdx = indexAt( event->pos());
        md.setInsertPos(currentIdx.row());
        md.setDestination(E_MEDIA_COL);

        qDebug() << QString("Move [%1,%2] to %3").arg(md.getSelectedItems().at(0).line).arg(md.getSelectedItems().at(md.getNumSelected() - 1 ).line).arg(md.getInsertPos());

        // plModel->makeDiff(&md);
        m_diffBuilder->diff(md);
    }
}

void CMediaColView::performDrag()
{
	qDebug() << QString("performDrag");

	CMuroaTreeModel* mcModel = reinterpret_cast<CMuroaTreeModel*>(model());

    QModelIndexList indexList = selectedIndexes();
    CModelDiff md(E_MEDIA_COL);

    // indexList contains all selected ModelIndexes, when a complete row is selected one per column.
    QList<int> rowsSeen;
    for(int i = 0; i < indexList.size(); i++)
    {
    	int pos = indexList.at(i).row();
    	if( ! rowsSeen.contains(pos))
    	{
    		rowsSeen.append(pos);
    		comb_hash_t combhash;
    		muroa::CItemBase* item = mcModel->itemFromIndex(indexList.at(i));
    		switch(item->type()) {
    		case CItemType::E_ROOT:
    			// add all
    			break;
    		case CItemType::E_CAT:
    			// add whole Category
    			break;

    		case CItemType::E_INVAL:
    			break;

    		default:
    			// derived from IContentitem
    			IContentItem* citem = reinterpret_cast<IContentItem*>(item);
    			combhash.type = citem->type();
    			combhash.hash = citem->getHash();
    			md.appendToSelectedItems(combhash);

    		}
    	}
    	else {
    		qDebug() << QString("duplicate row %1").arg(pos);
    	}
    }

    if (md.getNumSelected() > 0)
    {
    	QMimeData *mimeData = new QMimeData;
        //mimeData->setText(item->asString());
        mimeData->setData("application/x-muroa-playlist-diff", md.toQByteArray());

        m_dragActive = true;
        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        // drag->setPixmap(QPixmap(":/images/person.png"));

        Qt::DropAction action = drag->exec(Qt::MoveAction);

        m_dragActive = false;
        qDebug() << QString("DropAction: %1").arg(action);

    }
}
