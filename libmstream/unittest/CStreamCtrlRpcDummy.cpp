/*
 Copyright (c) 2002-2014 "Martin Runge"

 CStreamCtrl.cpp is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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

#include "CStreamCtrlRpcDummy.h"
#include <sstream>

using namespace std;

namespace muroa {

CStreamCtrlRpcDummy::CStreamCtrlRpcDummy() {

}

CStreamCtrlRpcDummy::CStreamCtrlRpcDummy(CStreamCtrlRpcDummy* receiver) : m_receiver(receiver) {

}


CStreamCtrlRpcDummy::~CStreamCtrlRpcDummy() {

}

void CStreamCtrlRpcDummy::onOpen(uint32_t cmdID) {
}

void CStreamCtrlRpcDummy::onClose(uint32_t cmdID) {
}

void CStreamCtrlRpcDummy::onAck(uint32_t cmdID) {
	m_last_ack = cmdID;
}

void CStreamCtrlRpcDummy::onError(uint32_t cmdID, int errorCode, std::string errmsg) {
	ostringstream oss;
	oss << "onError(" << cmdID << ", " << errorCode << ", '" << errmsg << "')";
	m_last_cmd = oss.str();
}

void CStreamCtrlRpcDummy::onJoinSession(uint32_t cmdID, std::string name, boost::asio::ip::address session_srv) {
	m_last_cmdID = cmdID;
	m_last_cmd = name;
	m_last_ip_addr = session_srv;
}

void CStreamCtrlRpcDummy::onJoinSessionLeave() {
}

void CStreamCtrlRpcDummy::onTakeFromSession(uint32_t cmdID, std::string name, boost::asio::ip::address session_srv) {
	m_last_cmdID = cmdID;
	m_last_cmd = name;
	m_last_ip_addr = session_srv;
}

void CStreamCtrlRpcDummy::onSetTimeSrv(uint32_t cmdID, boost::asio::ip::address session_srv, uint32_t port) {
	m_last_cmdID = cmdID;
	m_timesrv_ip_addr = session_srv;
	m_timesrv_port = port;
}

void CStreamCtrlRpcDummy::onGetTimeSrv(uint32_t cmdID) {
	setTimeSrv(cmdID, m_timesrv_ip_addr, m_timesrv_port);
}

void CStreamCtrlRpcDummy::onGetRTPPort(uint32_t cmdID) {
	setRTPPort(cmdID, m_rtp_port);
}

void CStreamCtrlRpcDummy::onSetRTPPort(uint32_t cmdID, uint32_t port) {
	m_last_cmdID = cmdID;
	m_rtp_port = port;
}

void CStreamCtrlRpcDummy::onJoinMCastGrp(uint32_t cmdID, boost::asio::ip::address mcast_addr) {
	m_last_cmdID = cmdID;
	m_mcast_grp = mcast_addr;
}

void CStreamCtrlRpcDummy::onLeaveMCastGrp(uint32_t cmdID, boost::asio::ip::address mcast_addr) {
	m_last_cmdID = cmdID;
	m_mcast_grp = mcast_addr;
}

void CStreamCtrlRpcDummy::onGetMCastGrp(uint32_t cmdID) {
	joinMCastGrp(cmdID, m_mcast_grp);
}

void CStreamCtrlRpcDummy::onSetStreamTimeBase(uint32_t cmdID, uint32_t ssrc, uint64_t rtp_ts, uint64_t pts) {
	m_last_cmdID = cmdID;
	m_timebase_ssrc = ssrc;
	m_timebase_rtp_ts = rtp_ts;
	m_timebase_pts = pts;
}

void CStreamCtrlRpcDummy::onGetStreamTimeBase(uint32_t cmdID, uint32_t ssrc) {
	setStreamTimeBase(cmdID, m_timebase_ssrc, m_timebase_rtp_ts, m_timebase_pts);
}

void CStreamCtrlRpcDummy::onResetStream(uint32_t cmdID, uint32_t ssrc) {
	ostringstream oss;
	m_last_cmdID = cmdID;
	m_last_ssrc = ssrc;

	oss << "onResetStream(" << cmdID << ", " << ssrc << ")";
	m_last_cmd = oss.str();
}

void CStreamCtrlRpcDummy::onGetVolume(uint32_t cmdID) {
	setVolume(cmdID, m_vol_percent);
}

void CStreamCtrlRpcDummy::onSetVolume(uint32_t cmdID, int percent) {
	m_last_cmdID = cmdID;
	m_vol_percent = percent;
}

void CStreamCtrlRpcDummy::onDataToSend(const char* data, int len) {
	m_receiver->newData(data, len);
}

string CStreamCtrlRpcDummy::getLastCmdInternal() {
	return m_last_cmd;
}

boost::asio::ip::address CStreamCtrlRpcDummy::getLastIpAddrInternal() const {
	return m_last_ip_addr;
}

uint32_t CStreamCtrlRpcDummy::getLastCmdIdInternal() const {
	return m_last_cmdID;
}

uint32_t CStreamCtrlRpcDummy::getLastSsrcInternal() const {
	return m_last_ssrc;
}

uint32_t CStreamCtrlRpcDummy::getLastPortInternal() const {
	return m_last_port;
}

const boost::asio::ip::address& CStreamCtrlRpcDummy::getMcastGrpInternal() const {
	return m_mcast_grp;
}

uint64_t CStreamCtrlRpcDummy::getTimebasePtsInternal() const {
	return m_timebase_pts;
}

void CStreamCtrlRpcDummy::setTimebasePtsInternal(uint64_t pts) {
	m_timebase_pts = pts;
}

uint64_t CStreamCtrlRpcDummy::getTimebaseRtpTsInternal() const {
	return m_timebase_rtp_ts;
}

void CStreamCtrlRpcDummy::setTimebaseRtpTsInternal(uint64_t rtp_ts) {
	m_timebase_rtp_ts = rtp_ts;
}

uint32_t CStreamCtrlRpcDummy::getTimebaseSsrcInternal() const {
	return m_timebase_ssrc;
}

void CStreamCtrlRpcDummy::setTimebaseSsrcInternal(uint32_t ssrc) {
	m_timebase_ssrc = ssrc;
}

const boost::asio::ip::address& CStreamCtrlRpcDummy::getTimesrvIpAddrInternal() const {
	return m_timesrv_ip_addr;
}

void CStreamCtrlRpcDummy::setTimesrvIpAddrInternal(const boost::asio::ip::address& ip_addr) {
	m_timesrv_ip_addr = ip_addr;
}

uint32_t CStreamCtrlRpcDummy::getTimesrvPortInternal() const {
	return m_timesrv_port;
}

void CStreamCtrlRpcDummy::setTimesrvPortInternal(uint32_t port) {
	m_timesrv_port = port;
}

uint32_t CStreamCtrlRpcDummy::getRtpPortInternal() const {
	return m_rtp_port;
}

void CStreamCtrlRpcDummy::setRtpPortInternal(uint32_t port) {
	m_rtp_port = port;
}

int CStreamCtrlRpcDummy::getVolPercentInternal() const {
	return m_vol_percent;
}

void CStreamCtrlRpcDummy::setVolPercentInternal(int percent) {
	m_vol_percent = percent;
}

void CStreamCtrlRpcDummy::setMCastGrpInternal(const boost::asio::ip::address& ip_addr) {
	m_mcast_addr = ip_addr;
}

boost::asio::ip::address CStreamCtrlRpcDummy::getMCastGrpInternal() {
	return m_mcast_addr;
}

uint32_t CStreamCtrlRpcDummy::getLastAck() const {
	return m_last_ack;
}

} /* namespace muroa */

