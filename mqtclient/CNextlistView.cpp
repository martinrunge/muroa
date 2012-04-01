/*
 * CNextlistView.cpp
 *
 *  Created on: 6 Jan 2010
 *      Author: martin
 */
#include <QtGui>
#include <QDebug>
#include <QMimeData>

#include "CNextlistView.h"
#include "CMuroaListModel.h"
#include "CPlaylistItem.h"

#include "CDiffBuilder.h"

CNextlistView::CNextlistView(QWidget * parent ) : QListView( parent ),
                                                  m_dragActive(false),
                                                  m_role(E_NEXTLIST) {
	setAcceptDrops(true);
	viewport()->setAcceptDrops(true);
	setDragDropMode(QAbstractItemView::DragDrop);
	setDropIndicatorShown(true);
	setSelectionMode(QAbstractItemView::ExtendedSelection);
	//setDragDropMode( QAbstractItemView::DragDrop );
}

CNextlistView::~CNextlistView() {
	// TODO Auto-generated destructor stub
}

void CNextlistView::mousePressEvent(QMouseEvent *event)
{
	qDebug() << QString("mousePressEvent");
    if (event->button() == Qt::LeftButton && !m_dragActive)
        m_startPos = event->pos();
    QListView::mousePressEvent(event);
}

void CNextlistView::mouseMoveEvent(QMouseEvent *event)
{
	qDebug() << QString("mouseMoveEvent");
    if (event->buttons() & Qt::LeftButton && !m_dragActive) {
        int distance = (event->pos() - m_startPos).manhattanLength();
        if (distance >= QApplication::startDragDistance())
            performDrag();
    }
    //QListView::mouseMoveEvent(event);
}

void CNextlistView::dragEnterEvent(QDragEnterEvent *event)
{
	qDebug() << QString("dragEnterEvent");
    //CNextlistView *source = qobject_cast<CNextlistView *>(event->source());
	const QMimeData* data = event->mimeData();

    if (data && data->hasFormat("application/x-muroa-playlist-diff")) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void CNextlistView::dragMoveEvent(QDragMoveEvent *event)
{
	qDebug() << QString("dragMoveEvent");
    // CNextlistView *source = qobject_cast<CNextlistView *>(event->source());
	const QMimeData* data = event->mimeData();

    if (data && data->hasFormat("application/x-muroa-playlist-diff")) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void CNextlistView::dropEvent(QDropEvent *event)
{
	qDebug() << QString("dropEvent");
	const QMimeData* data = event->mimeData();

    if (data && data->hasFormat("application/x-muroa-playlist-diff")) {
        event->setDropAction(Qt::MoveAction);
        event->accept();

        // CPlaylistModel* plModel = reinterpret_cast<CPlaylistModel*>(model());

        CModelDiff md(data->data("application/x-muroa-playlist-diff"));
        QModelIndex currentIdx = indexAt( event->pos());
        //md.appendToInsert(currentIdx.row(), plModel->itemAt(currentIdx.row())->getHash());
        int insertPos = currentIdx.row();
        if(insertPos == -1)
        {
        	insertPos = model()->rowCount();
        }
        md.setInsertPos( insertPos );
        md.setDestination(m_role);

        qDebug() << QString("CNextlistView::dropEvent: Move [%1,%2] to %3").arg(md.getSelectedItems().at(0).hash).arg(md.getSelectedItems().at(md.getNumSelected() - 1 ).hash).arg(md.getInsertPos());
        // plModel->makeDiff(&md);
        m_diffBuilder->diff(md);
    }
}

void CNextlistView::performDrag()
{
	qDebug() << QString("performDrag");

    CMuroaListModel* plModel = reinterpret_cast<CMuroaListModel*>(model());

    QModelIndexList indexList = selectedIndexes();
    CModelDiff md(m_role);

    for(int i = 0; i < indexList.size(); i++)
    {
    	comb_hash_t combhash;
		CItemBase* item = plModel->itemFromIndex(indexList.at(i));
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
			combhash.line = indexList.at(i).row();
			md.appendToSelectedItems(combhash);
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
