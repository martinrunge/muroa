/*
 * RootItemIterator.cpp
 *
 *  Created on: 10 Dec 2011
 *      Author: martin
 */

// #include "CCategoryItemIterator.h"

#include "CItemBase.h"
#include "CCategoryItem.h"
#include "CRootItem.h"
#include <cassert>

namespace muroa {

CRootItemIterator::CRootItemIterator(CCategoryItem* base, int pos) : m_base(base),
		                                                             m_pos(pos),
		                                                             m_base_stack(),
		                                                             m_pos_stack()
{
	if(m_pos == 0) {
		if(m_base->numChildren() > 0) {
			CItemBase* item = m_base->childAt(0);
			while(item->type() == CItemType::E_CAT) {
				m_base_stack.push(m_base);
				m_base = reinterpret_cast<CCategoryItem*>(item);
				m_pos_stack.push(0);
				item = m_base->childAt(0);
			}
		}
	}
}

CRootItemIterator::CRootItemIterator(const CRootItemIterator& it) : m_base(it.m_base),
                                                                    m_pos(it.m_pos),
                                                                    m_base_stack(it.m_base_stack ),
                                                                    m_pos_stack(it.m_pos_stack )

{}

CRootItemIterator& CRootItemIterator::operator++() {
	assert( m_pos_stack.size() == (m_base_stack.size() ));

	++m_pos;
	while( m_pos >= m_base->numChildren() && !m_pos_stack.empty()) {
		// go back one in increment that, possibly descending in another leave
		m_pos = m_pos_stack.top();
		m_pos_stack.pop();
		m_base = m_base_stack.top();
		m_base_stack.pop();
		++m_pos;
	}

	if( m_pos < m_base->numChildren() ) { // do not ask for m_base->childAt( "too big" ) => that's itertor::end()
		CItemBase* item = m_base->childAt(m_pos);
		while(item->type() == CItemType::E_CAT) {
			m_base_stack.push(m_base);
			m_base = reinterpret_cast<CCategoryItem*>(item);
			m_pos_stack.push(m_pos);
			m_pos = 0;
			item = m_base->childAt(m_pos);
		}
	}
	return *this;
}

CRootItemIterator CRootItemIterator::operator++(int) {
	CRootItemIterator tmp(*this);
	this->operator++();
	return tmp;
}


bool CRootItemIterator::operator==(const CRootItemIterator& rhs) {
	assert( m_pos_stack.size() == (m_base_stack.size() ));
	assert( rhs.m_pos_stack.size() == (rhs.m_base_stack.size() ));

	if( m_base != rhs.m_base) return false;
	if( m_pos != rhs.m_pos) return false;

	return ( m_base_stack == rhs.m_base_stack && m_pos_stack == rhs.m_pos_stack );
}

bool CRootItemIterator::operator!=(const CRootItemIterator& rhs) {
	return (!operator==(rhs));
}

CItemBase* CRootItemIterator::operator*() {
	return m_base->childAt(m_pos);
}

}
