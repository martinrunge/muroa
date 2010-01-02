/*
 * CItemBase.h
 *
 *  Created on: 28 Nov 2009
 *      Author: martin
 */

#ifndef CITEMBASE_H_
#define CITEMBASE_H_

#include <QString>

class CItemBase {
public:
	CItemBase();
	virtual ~CItemBase();

//	void setText(QString text);
	inline QString getText() const { return m_text; };

protected:
    QString m_text;

};

#endif /* CITEMBASE_H_ */
