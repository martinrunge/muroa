/***************************************************************************
 *
 *   asio-watch.cpp
 *
 *   This file is part of Asiotest                                         *
 *   Copyright (C) 2011 by Martin Runge <martin.runge@web.de>              *
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

#include <sys/time.h>
#include <avahi-common/timeval.h>

#include "asio-watch.h"

using boost::asio::io_service;

class AsioWatch {
public:
	AsioWatch(int fd, AvahiWatchEvent event, AvahiWatchCallback callback, void* userdata);
	virtual ~AsioWatch();
    AvahiWatchEvent getEvents() const { return m_incallback ? m_lastEvent : (AvahiWatchEvent)0; }
    void setWatchedEvents(AvahiWatchEvent event);

private:
    void in_handler();
    void out_handler();

//    QSocketNotifier* m_in;
//    QSocketNotifier* m_out;
    //FIXME: ERR and HUP?
    AvahiWatchCallback m_callback;
    AvahiWatchEvent m_lastEvent;
    int m_fd;
    void* m_userdata;
    bool m_incallback;
};


AsioWatch::AsioWatch(int fd, AvahiWatchEvent event, AvahiWatchCallback callback, void* userdata):
    m_callback(callback), m_fd(fd), m_userdata(userdata), m_incallback(false)
{
    setWatchedEvents(event);
}

AsioWatch::~AsioWatch() {
}


void AsioWatch::in_handler() {
    m_lastEvent = AVAHI_WATCH_IN;
    m_incallback=true;
    m_callback(this, m_fd, m_lastEvent, m_userdata);
    m_incallback=false;
}

void AsioWatch::out_handler() {
    m_lastEvent = AVAHI_WATCH_IN;
    m_incallback=true;
    m_callback(this, m_fd, m_lastEvent, m_userdata);
    m_incallback=false;
}

void AsioWatch::setWatchedEvents(AvahiWatchEvent event)  {
    if (!(event & AVAHI_WATCH_IN)) { delete m_in; m_in=0; }
    if (!(event & AVAHI_WATCH_OUT)) { delete m_out; m_out=0; }
    if (event & AVAHI_WATCH_IN) {
    	m_in = new QSocketNotifier(m_fd,QSocketNotifier::Read, this);
    	connect(m_in,SIGNAL(activated(int)),SLOT(gotIn()));
    }
    if (event & AVAHI_WATCH_OUT) {
    	m_out = new QSocketNotifier(m_fd,QSocketNotifier::Write, this);
    	connect(m_out,SIGNAL(activated(int)),SLOT(gotOut()));
    }
}






class AsioTimeout {
public:
	AsioTimeout(const struct timeval* tv, AvahiTimeoutCallback callback, void* userdata);
	virtual ~AsioTimeout();
    void update(const struct timeval* tv);

private:
    void timeout(const boost::system::error_code& error);

    boost::asio::deadline_timer m_timer;
    AvahiTimeoutCallback m_callback;
    void* m_io_service_ptr;
};



AsioTimeout::AsioTimeout(const struct timeval* tv, AvahiTimeoutCallback callback, io_service* io_service_ptr) :
		m_timer(*io_service_ptr), m_callback(callback), m_io_service_ptr(io_service_ptr)
{
    update(tv);
}

void AsioTimeout::update(const struct timeval *tv) {
    m_timer.cancel();
    if (tv) {
    	AvahiUsec u = avahi_age(tv)/1000;
    	m_timer.start( (u>0) ? 0 : -u);
    	m_timer.async_wait(boost::bind(&AsioTimeout::timeout, boost::asio::placeholders::error, this));
    }
}

void AsioTimeout::timeout(const boost::system::error_code& error) {
    if (error == boost::asio::error::operation_aborted) {
        std::cout << "Timer was canceled" << std::endl;
    }
    else if (error) {
        std::cout << "Timer error: " << error.message() << std::endl;
        m_callback(this, m_io_service_ptr);
    }
}



static AvahiWatch* asio_watch_new(const AvahiPoll *api, int fd, AvahiWatchEvent event, AvahiWatchCallback callback, void *userdata) {
    return new AsioWatch(fd, event, callback, userdata);
}

static void asio_watch_update(AvahiWatch *w, AvahiWatchEvent events)
{
    w->setWatchedEvents(events);
}

static AvahiWatchEvent asio_watch_get_events(AvahiWatch *w)
{
    return w->getEvents();
}

static void asio_watch_free(AvahiWatch *w)
{
    delete w;
}

static AvahiTimeout* asio_timeout_new(const AvahiPoll *api, const struct timeval *tv, AvahiTimeoutCallback callback,
    void *userdata)
{
    return new AsioTimeout(tv, callback, (io_service*)userdata);
}

static void asio_timeout_update(AvahiTimeout *t, const struct timeval *tv)
{
    t->update(tv);
}

static void asio_timeout_free(AvahiTimeout *t)
{
    delete t;
}

const AvahiPoll* avahi_asio_poll_get(io_service *io_service_ptr)
{
    static const AvahiPoll asio_poll = {
   		io_service_ptr,
        asio_watch_new,
        asio_watch_update,
        asio_watch_get_events,
        asio_watch_free,
        asio_timeout_new,
        asio_timeout_update,
        asio_timeout_free
    };

    return &asio_poll;
}
