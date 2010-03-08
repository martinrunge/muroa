/*
 * CCollectionView.cpp
 *
 *  Created on: 23 Jan 2010
 *      Author: martin
 */
#include <QtGui>
#include <QDebug>
#include <QMimeData>

#include "CCollectionView.h"
#include "CCollectionModel.h"
#include "CCollectionItem.h"

#include "CModelDiff.h"
#include "CDiffBuilder.h"


CCollectionView::CCollectionView(QWidget * parent) : QTreeView(parent){
	// TODO Auto-generated constructor stub

}

CCollectionView::~CCollectionView() {
	// TODO Auto-generated destructor stub
}

void CCollectionView::mousePressEvent(QMouseEvent *event)
{
	qDebug() << QString("mousePressEvent");
    if (event->button() == Qt::LeftButton && !m_dragActive)
        m_startPos = event->pos();
    QTreeView::mousePressEvent(event);
}

void CCollectionView::mouseMoveEvent(QMouseEvent *event)
{
	qDebug() << QString("mouseMoveEvent");
    if (event->buttons() & Qt::LeftButton && !m_dragActive) {
        int distance = (event->pos() - m_startPos).manhattanLength();
        if (distance >= QApplication::startDragDistance())
            performDrag();
    }
    //QTreeView::mouseMoveEvent(event);
}

void CCollectionView::dragEnterEvent(QDragEnterEvent *event)
{
	qDebug() << QString("dragEnterEvent");
    //CPlaylistView *source = qobject_cast<CPlaylistView *>(event->source());
	const QMimeData* data = event->mimeData();

    if (data && data->hasFormat("application/x-muroa-playlist-diff")) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void CCollectionView::dragMoveEvent(QDragMoveEvent *event)
{
	qDebug() << QString("dragMoveEvent");
    // CPlaylistView *source = qobject_cast<CPlaylistView *>(event->source());
	const QMimeData* data = event->mimeData();

    if (data && data->hasFormat("application/x-muroa-playlist-diff")) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void CCollectionView::dropEvent(QDropEvent *event)
{
	qDebug() << QString("dropEvent");
	const QMimeData* data = event->mimeData();

    if (data && data->hasFormat("application/x-muroa-playlist-diff")) {
        event->setDropAction(Qt::MoveAction);
        event->accept();

        CCollectionModel* plModel = reinterpret_cast<CCollectionModel*>(model());

        CModelDiff md(data->data("application/x-muroa-playlist-diff"));
        QModelIndex currentIdx = indexAt( event->pos());
        md.setInsertPos(currentIdx.row());
        md.setDestination(E_COLLECTION);

        qDebug() << QString("Move [%1,%2] to %3").arg(md.getSelectedIndexes().at(0)).arg(md.getSelectedIndexes().at(md.getNumSelected() - 1 )).arg(md.getInsertPos());

        // plModel->makeDiff(&md);
        m_diffBuilder->diff(md);
    }
}

void CCollectionView::performDrag()
{
	qDebug() << QString("performDrag");

	CCollectionModel* plModel = reinterpret_cast<CCollectionModel*>(model());

    QModelIndexList indexList = selectedIndexes();
    CModelDiff md(E_COLLECTION);

    // indexList contains all selected ModelIndexes, when a complete row is selected one per column.
    QList<int> rowsSeen;

    for(int i = 0; i < indexList.size(); i++)
    {
    	int pos = indexList.at(i).row();
    	if( ! rowsSeen.contains(pos))
    	{
    		rowsSeen.append(pos);
    		md.appendToSelectedIndexes(pos);
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
