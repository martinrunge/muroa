/***************************************************************************
 *
 *   asio-watch.h
 *
 *   This file is part of Asiotest                                  *
 *   Copyright (C) 2011 by Martin Runge <martin.runge@web.de>           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef ASIO_WATCH_H_
#define ASIO_WATCH_H_

#include <avahi-common/watch.h>
#include <boost/asio.hpp>

AVAHI_C_DECL_BEGIN

/** Setup abstract poll structure for integration with Qt main loop  */
const AvahiPoll* avahi_asio_poll_get(boost::asio::io_service& io_service)
#ifdef HAVE_VISIBILITY_HIDDEN
__attribute__ ((visibility("default")))
#endif
;

AVAHI_C_DECL_END

#endif /* ASIO_WATCH_H_ */
