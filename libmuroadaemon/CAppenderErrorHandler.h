/*
 Copyright (c) 2002-2014 "Martin Runge"

 CAppenderErrorHandler.h is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

 Muroa is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */

#ifndef CAPPENDERERRORHANDLER_H_
#define CAPPENDERERRORHANDLER_H_

#include <log4cplus/appender.h>

namespace muroa {

class CAppenderErrorHandler : public log4cplus::ErrorHandler {
public:
	CAppenderErrorHandler();
	virtual ~CAppenderErrorHandler();

	void error(const log4cplus::tstring &err);

	void reset();

};

} /* namespace muroa */

#endif /* CAPPENDERERRORHANDLER_H_ */
