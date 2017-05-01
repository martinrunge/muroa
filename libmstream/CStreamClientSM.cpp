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
#include <Exceptions.h>

namespace muroa {

char const* const CStreamClientSM::outer_state_names[] = {  "awaitClientState", "knowingClientState", "awaitJoinResponse", "joinedSession", "error" };
char const* const CStreamClientSM::inner_state_names[] = {  "awaitSessionState", "sessionMember", "noError", "errorExit" };

class IClientSMActions;

CStreamClientSM::CStreamClientSM(IClientSMActions* actions) {
	_actions = actions;
}

CStreamClientSM::~CStreamClientSM() {
	// TODO Auto-generated destructor stub
}

/// @brief: cast events to their types and process them
//
// this is needed, because boost::msm's process_event method takes the event as template parameter.
// No need for a common base type, but the actual type must be known at compile time because of
// template instantiation.
bool CStreamClientSM::onEvent(muroa::CmdStreamBase* ev) {

    if      ( typeid(*ev) == typeid(evRequestJoin) )     process_event( *reinterpret_cast<evRequestJoin*>(ev) );
	else if ( typeid(*ev) == typeid(evRequestLeave) )    process_event( *reinterpret_cast<evRequestLeave*>(ev) );
	else if ( typeid(*ev) == typeid(evJoinRejected) )    process_event( *reinterpret_cast<evJoinRejected*>(ev) );
	else if ( typeid(*ev) == typeid(evGetSessionState) ) process_event( *reinterpret_cast<evGetSessionState*>(ev) );
	else if ( typeid(*ev) == typeid(evSessionState) )    process_event( *reinterpret_cast<evSessionState*>(ev) );
	else if ( typeid(*ev) == typeid(evResetStream) )     process_event( *reinterpret_cast<evResetStream*>(ev) );
	else if ( typeid(*ev) == typeid(evSyncStream) )      process_event( *reinterpret_cast<evSyncStream*>(ev) );
	else if ( typeid(*ev) == typeid(evSetVolume) )       process_event( *reinterpret_cast<evSetVolume*>(ev) );
	else if ( typeid(*ev) == typeid(evVolume) )          process_event( *reinterpret_cast<evVolume*>(ev) );
	else if ( typeid(*ev) == typeid(evAck) )             process_event( *reinterpret_cast<evAck*>(ev) );
	else if ( typeid(*ev) == typeid(evError) )           process_event( *reinterpret_cast<evError*>(ev) );
	else {
		throw CException("CStreamClientSM::onEvent: unknown event type");
	}
}


void CStreamClientSM::activeStates(VisitorBase& vis) {
	visit_current_states(boost::ref(vis));
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
