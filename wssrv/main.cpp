/*
Copyright (c) 2002-2014 "Martin Runge"

main.cpp is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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

#include <iostream>
#include <getopt.h>
#include <vector>
#include <string>

#include "CApp.h"
#include "CUtils.h"

#include <boost/asio.hpp>
#include <boost/filesystem.hpp>

#include "CppServer.h"
#include "CRessourceHandler.h"
#include "CWSMsgHandler.h"
#include "WSSrv.h"

using namespace std;

void usage(string progname)
{
    cout << "usage: " << progname << " [options]" << endl
         << "options: " << endl
         << "  --muroad    -m <client1:port>   a render client in the form hostname:port, option can be repeated for two or more clients"  << endl
         << "  --tsport    -t <num>   port used by timeserver" << endl
         << "  --sessionid -s <num>   a numerical session id" << endl;

}

bip::tcp::endpoint string_to_endpoint(std::string addr_str, int default_port) {
	bip::tcp::endpoint endp;

	string portstr;

	int p1 = addr_str.find('[');
	int p2 = addr_str.find(']');

	if( p1 != string::npos && p2 != string::npos) {
		// IPv6 Address in square brackets, possibly with port nr
		string ip6_addr_str = addr_str.substr(p1, p2);
		unsigned long portNr = default_port;

		int colonpos = addr_str.find(':', p2);
		if(colonpos != string::npos) {
			portstr = addr_str.substr( colonpos + 1);
			portNr = strtoul(portstr.c_str(), NULL, 10);
		}
		boost::asio::ip::address ip_addr = bip::address::from_string(ip6_addr_str);
		endp = bip::tcp::endpoint(ip_addr, portNr);
	}
	else {
		int firstcolonpos = addr_str.find(':');
		if( firstcolonpos != string::npos ) {
			// at least one colon inside
			int second_colon = addr_str.find(':', firstcolonpos + 1);
			if(second_colon == string::npos) {
				// IPv4 address with port number
				portstr = addr_str.substr(firstcolonpos + 1);
				unsigned long portNr = strtoul(portstr.c_str(), NULL, 10);

				string ip_addr_str = addr_str.substr(0, firstcolonpos);
				boost::asio::ip::address ip_addr = bip::address::from_string(ip_addr_str);
				endp = bip::tcp::endpoint(ip_addr, portNr);
			}
			else {
				// IPv6 address without square brackets / port number
				boost::asio::ip::address ip_addr = bip::address::from_string(addr_str);
				endp = bip::tcp::endpoint(ip_addr, default_port);
			}
		}
		else {
			//string is an IPv4 address without port
			boost::asio::ip::address ip_addr = bip::address::from_string(addr_str);
			endp = bip::tcp::endpoint(ip_addr, default_port);
		}
	}

	return endp;
}

class CSigHandler {
public:
	CSigHandler( boost::asio::io_service& io_service) : m_io_service(io_service) {
	    boost::asio::signal_set signals(m_io_service, SIGINT, SIGTERM);
	    signals.async_wait(bind(&CSigHandler::onSignal,this,::_1,::_2));
	};
	~CSigHandler() {};

	void onSignal( const boost::system::error_code& error, int signal_number) {
		if (!error)
		{
			m_io_service.stop();
		}
	}

private:
	boost::asio::io_service& m_io_service;
};


int main(int argc, char *argv[]) {
//  cserver(argc, argv);
    char c;
    int verbose_flag = 0;

    vector<string> clients;
    int sessionID = -1;
    int timeSrvPort = -1;

    muroa::CApp* app;

	try {
		app = muroa::CApp::getInstPtr(argc, argv);
	}
	catch (muroa::configEx& ex) {
		cerr << "got exception configEx: " << ex.what() << endl;
		return EXIT_FAILURE;
	}

	if(!app->settings().foreground()) {
		app->daemonize();
	}

	clients = app->settings().muroad_addrs();

	if(app->settings().timeserver_port() != 0) {
		timeSrvPort = app->settings().timeserver_port();
	}

	if(clients.size() == 0) {
		cerr << "error: no clients listed. Please list at least one stream client by its service name." << endl;
	}
		//        // bip::tcp::endpoint endp = string_to_endpoint("127.0.0.1", 5556);
//        clients.push_back(endp);
//    }

	boost::filesystem::path docroot = app->settings().getConfigVal("muroaws.docroot", "/var/www/muroa");
	docroot = CUtils::expandvars(docroot);
	if(!docroot.is_absolute()) {
		docroot = app->getAbsProgDir() / docroot;
	}

	boost::filesystem::path stations_file = docroot / "stations.json";
	Json::Reader reader;
	Json::Value stations;
	ifstream ifs;
	ifs.open(stations_file.string());
	reader.parse(ifs, stations);
	ifs.close();

	boost::asio::io_service io_service;

	CppServer cpps(io_service, clients, "testsession", timeSrvPort, sessionID);
	CRessourceHandler resHandler(&cpps, stations);
	CWSMsgHandler wsMsgHandler(&cpps, stations);

    CSigHandler sig_handler(io_service);

    muroa::WSSrv wssrv(&io_service, &resHandler, &wsMsgHandler);

    wsMsgHandler.setWSSrv(&wssrv);

    cpps.setWSMsgHandler(&wsMsgHandler);

    wssrv.run(docroot.string(), 8888);

    io_service.run();
	return 0;
}
