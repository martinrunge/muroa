/*
 * CRenderClientDesc.cpp
 *
 *  Created on: 16.05.2016
 *      Author: martin
 */

#include "CRenderClientDesc.h"

namespace muroa {

CRenderClientDesc::CRenderClientDesc() {
	// TODO Auto-generated constructor stub

}

CRenderClientDesc::~CRenderClientDesc() {
	// TODO Auto-generated destructor stub
}


const std::string& muroa::CRenderClientDesc::getServiceName() const {
	return m_service_name;
}

void muroa::CRenderClientDesc::setServiceName(const std::string& serviceName) {
	m_service_name = serviceName;
}

} /* namespace muroa */
