/*
 * CRenderClientDesc.h
 *
 *  Created on: 16.05.2016
 *      Author: martin
 */

#ifndef LIBMSTREAM_CRENDERCLIENTDESC_H_
#define LIBMSTREAM_CRENDERCLIENTDESC_H_

#include <string>

namespace muroa {

class CRenderClientDesc {
public:
	CRenderClientDesc();
	virtual ~CRenderClientDesc();

	const std::string& getServiceName() const;
	void setServiceName(const std::string& serviceName);

private:
	std::string m_service_name;

};

} /* namespace muroa */

#endif /* LIBMSTREAM_CRENDERCLIENTDESC_H_ */
