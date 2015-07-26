/*
 Copyright (c) 2002-2014 "Martin Runge"

 CStreamClientSM.cpp is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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

#include <CStreamClientSM.h>

namespace muroa {

char const* const CStreamClientSM::outer_state_names[] = {  "awaitClientState", "knowingClientState", "awaitJoinResponse", "joinedSession", "error" };
char const* const CStreamClientSM::inner_state_names[] = {  "awaitSessionState", "sessionMember", "noError", "errorExit" };


CStreamClientSM::CStreamClientSM() {
	// TODO Auto-generated constructor stub

}

CStreamClientSM::~CStreamClientSM() {
	// TODO Auto-generated destructor stub
}

void CStreamClientSM::pstate() {
    typedef msm::back::generate_state_set<stt>::type all_states;
    static char const* state_names[mpl::size<all_states>::value];

    // fill the names of the states defined in the state machine
    mpl::for_each<all_states,boost::msm::wrap<mpl::placeholders::_1> >
        (msm::back::fill_state_names<stt>(state_names));

    for (unsigned int i=0;i<nr_regions::value;++i)
    {
        std::cout << " -> " << state_names[current_state()[i]] << std::endl;
    }

    // std::cout << " -> (" << outer_state_names[current_state()[0]] << ", " << inner_state_names[current_state()[1]] << ")" << std::endl;
}

} /* namespace muroa */
