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

#include <boost/asio.hpp>
#include "server.h"

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

int main(int argc, char *argv[]) {
//  cserver(argc, argv);
    char c;
    int verbose_flag = 0;

    vector<bip::tcp::endpoint> clients;
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

    while (1)
      {
        static struct option long_options[] =
          {
            /* These options set a flag. */
            {"verbose", no_argument,       &verbose_flag, 1},
            /* These options don't set a flag.
               We distinguish them by their indices. */
            {"muroad",      required_argument, 0, 'm'},
            {"tsport",      required_argument, 0, 't'},
            {"sessionid",   required_argument, 0, 's'},
            {0, 0, 0, 0}
          };
        /* getopt_long stores the option index here. */
        int option_index = 0;

        c = getopt_long (argc, argv, "c:s:t:",
                         long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
          break;

        switch (c)
          {
          case 0:
            /* If this option set a flag, do nothing else now. */
            if (long_options[option_index].flag != 0)
              break;
            printf ("option %s", long_options[option_index].name);
            if (optarg)
              printf (" with arg %s", optarg);
            printf ("\n");
            break;

          case 'c':
          {
            cerr <<"option -c with value '" <<  optarg << "'"<< endl;
            bip::tcp::endpoint endp = string_to_endpoint(optarg, 5556);
            clients.push_back(endp);
            break;
          }
          case 's':
          {
            sessionID = strtol(optarg, NULL, 10);
            cerr << "option -s with value '"<< sessionID << "'" << endl;
            break;
          }
          case 't':
          {
            timeSrvPort = strtol(optarg, NULL, 10);
            cerr << "option -t with value '"<< timeSrvPort << "'" << endl;
            break;
          }
          case 'h':
          default:
            usage(argv[0]);
            exit(0);
          }
      }

    if(clients.size() == 0) {
        bip::tcp::endpoint endp = string_to_endpoint("127.0.0.1", 5556);
        clients.push_back(endp);
    }

    cppserver cpps(clients, timeSrvPort, sessionID);
    cpps.run();

	return 0;
}
