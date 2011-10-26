/*
 * CApp.h
 *
 *  Created on: 23 Oct 2011
 *      Author: martin
 */

#ifndef CAPP_H_
#define CAPP_H_

#include <mutex>
class CSettings;

namespace muroa {

class CApp {
private:
	CApp();
	virtual ~CApp();
	CSettings* getSettingsPtr(void);
	void setSettingsPtr(CSettings* settings_ptr);
public:
	static CApp* getInstPtr();

	void serviceChanged();
	void notifyService(CServiceDesc servdesc);

private:
	static CApp* m_inst_ptr;
	static std::mutex m_mutex;

	CSettings* m_settings_ptr;
};

} /* namespace muroa */
#endif /* CAPP_H_ */
