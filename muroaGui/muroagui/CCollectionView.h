/*
 * CCollectionView.h
 *
 *  Created on: 23 Jan 2010
 *      Author: martin
 */

#ifndef CCOLLECTIONVIEW_H_
#define CCOLLECTIONVIEW_H_

#include <QTreeView>

class CCollectionView : public QTreeView {
public:
	CCollectionView(QWidget * parent = 0);
	virtual ~CCollectionView();
};

#endif /* CCOLLECTIONVIEW_H_ */
