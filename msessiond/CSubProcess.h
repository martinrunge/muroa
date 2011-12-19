/*
 * CSubProcess.h
 *
 *  Created on: 23 Oct 2010
 *      Author: martin
 */

#ifndef CSUBPROCESS_H_
#define CSUBPROCESS_H_

#include <vector>
#include <string>

#include <thread>

#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

class CSubProcess {
public:
	CSubProcess(boost::asio::io_service& io_service);
	virtual ~CSubProcess();

	pid_t start(std::string executable, std::vector<std::string> args, int cpu_prio, int io_prio);

	inline bool isRunning() { return m_running; };

	int waitForExit(int timeout_in_ms = 0);
	int sendSignal(int sig_no);
	inline int kill() { return sendSignal(9); }

	void toStdin(const char* buffer, int length);

	inline std::string getLastErrorMsg() { return m_last_error_msg; };
	inline int getLastErrorCode() { return m_last_error_code; };

protected:
	virtual void fromStdout(const char* buffer, int length);
	virtual void fromStderr(const char* buffer, int length);

	void waitPid();
	virtual void finished();

	void writtenToStdin(const boost::system::error_code& error, size_t bytes_transferred);

private:
	boost::asio::io_service& m_io_service;

	int m_socket;
    boost::array<char, 8192> m_stdout_buffer;
    boost::array<char, 8192> m_stderr_buffer;


	int m_childs_stdin;
	int m_childs_stdout;
	int m_childs_stderr;

	boost::asio::local::stream_protocol::socket m_stdin_sock;
	boost::asio::local::stream_protocol::socket m_stdout_sock;
	boost::asio::local::stream_protocol::socket m_stderr_sock;

	void watchStdout();
	void watchStderr();

	void readFromStdout(const boost::system::error_code& error, size_t bytes_transferred);
	void readFromStderr(const boost::system::error_code& error, size_t bytes_transferred);

	pid_t m_pid;

	std::thread *m_waitthread;
	bool m_running;

	int m_last_error_code;
	std::string m_last_error_msg;


	FILE* m_dbg_file;
};

#endif /* CSUBPROCESS_H_ */
