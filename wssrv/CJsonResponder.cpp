/*
 * CJsonResponder.cpp
 *
 *  Created on: 13.12.2015
 *      Author: martin
 */

#include "CJsonResponder.h"
#include <iostream>
#include <jsoncpp/json/json.h>

namespace muroa {

using namespace std;

CJsonResponder::CJsonResponder() {

}

CJsonResponder::~CJsonResponder() {
	// TODO Auto-generated destructor stub
}

std::string CJsonResponder::testOutput() {
	Json::Value root;   // 'root' will contain the root value after parsing.

	// and return 'UTF-8' if there is no such member.
	std::string encoding = root.get("encoding", "UTF-8" ).asString();
	// Get the value of the member of root named 'plug-ins'; return a 'null' value if
	// necessary to explicitly construct the Json::Value object.
	root["encoding"] = "UTF-8";
	root["indent"]["length"] = 8;
	root["indent"]["use_space"] = "yes";
	// If you like the defaults, you can insert directly into a stream.
	std::cout << root;
	// Of course, you can write to `std::ostringstream` if you prefer.
	// If desired, remember to add a linefeed and flush.
	std::cout << std::endl;

	return string();
}

} /* namespace muroa */
