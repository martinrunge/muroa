/*
 * RootItemIterator.h
 *
 *  Created on: 10 Dec 2011
 *      Author: martin
 */

#ifndef CROOTITEMITERATOR_H_
#define CROOTITEMITERATOR_H_

#include <iterator>
#include <deque>
#include <stack>

#include "CItemBase.h"

#include "CCategoryItem.h"

namespace muroa {

class CRootItemIterator : public std::iterator<std::bidirectional_iterator_tag, CItemBase*>
{
	int m_pos;
	CCategoryItem* m_base;

	std::stack<int> m_pos_stack;
	std::stack<CCategoryItem*> m_base_stack;

public:
  CRootItemIterator(CCategoryItem* base, int pos = 0);
  CRootItemIterator(const CRootItemIterator& it);
  CRootItemIterator& operator++();
  CRootItemIterator operator++(int);
  bool operator==(const CRootItemIterator& rhs);
  bool operator!=(const CRootItemIterator& rhs);
  CItemBase* operator*();
};

} /* namespace muroa */
#endif /* CROOTITEMITERATOR_H_ */
