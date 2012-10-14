/*
 * IItemModel.h
 *
 *  Created on: 30 Sep 2012
 *      Author: martin
 */

#ifndef IITEMMODEL_H_
#define IITEMMODEL_H_

namespace muroa {

class CCategoryItem;

class IItemModel {
public:
	virtual ~IItemModel() {};

	virtual bool beginInsertItems(const int pos, const int count, const CCategoryItem* parent) = 0;
	virtual bool endInsertItems(void) = 0;
	virtual bool beginRemoveItems(const int pos, const int count, const CCategoryItem* parent) = 0;
	virtual bool endRemoveItems(void) = 0;

protected:
    virtual void reset() = 0;

};

} /* namespace muroa */
#endif /* IITEMMODEL_H_ */
