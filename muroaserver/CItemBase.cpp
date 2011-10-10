/*
 * CItemBase.cpp
 *
 *  Created on: 28 Nov 2009
 *      Author: martin
 */

#include "CItemBase.h"

CItemBase::CItemBase() {
}

CItemBase::~CItemBase() {
}

CItemBase::CItemBase(const CItemBase& other) {
	m_text = other.m_text;
	m_hash = other.m_hash;
}

//void CItemBase::setText(QString text) {
//	m_text = text;
//}

