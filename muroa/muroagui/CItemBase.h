/*
 * CItemBase.h
 *
 *  Created on: 23 Nov 2009
 *      Author: martin
 */

#ifndef CITEMBASE_H_
#define CITEMBASE_H_

#include <QVariant>
#include <QString>

class CItemBase {
public:
	CItemBase(QString itemStr);
	virtual ~CItemBase();

	inline QString asString() const { return m_as_string; };
    inline unsigned getHash() { return m_hash; };

	static QString getTitle(int col) { return QString(); };
	static int getNumColumns() { return 0; };

	virtual QVariant data(int column) const = 0;


protected:
	QString m_as_string;
	unsigned  m_hash;

};

#endif /* CITEMBASE_H_ */
