/*
 Copyright (c) 2002-2014 "Martin Runge"

 CStreamSrvSM.h is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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

#ifndef LIBMSTREAM_CSTREAMSRVSM_H_
#define LIBMSTREAM_CSTREAMSRVSM_H_

// msm back-end
#include <boost/msm/back/state_machine.hpp>
//msm front-end
#include <boost/msm/front/state_machine_def.hpp>
// functors
#include <boost/msm/front/functor_row.hpp>
#include <boost/msm/front/euml/common.hpp>
// for And_ operator
#include <boost/msm/front/euml/operator.hpp>
// for func_state and func_state_machine
#include <boost/msm/front/euml/state_grammar.hpp>

namespace msm = boost::msm;
namespace mpl = boost::mpl;
using namespace msm::front;
// for And_ operator
using namespace msm::front::euml;

#include "cmds/cmds.h"

using namespace muroa;

namespace muroa {

class CStreamSrvSM {
public:
	CStreamSrvSM();
	virtual ~CStreamSrvSM();


protected:
	// states: awaitClientState, knowingClientState, awaitJoinResponse, joinedSession, error
    // front-end: define the FSM structure
    struct srv_ : public msm::front::state_machine_def<srv_>
    {
        // The list of FSM states

    	// definition of awaitClientState
        // entry and exit functors for awaitClientState
        struct awaitClientState_Entry
        {
            template <class Event,class FSM,class STATE>
            void operator()(Event const&,FSM&,STATE& )
            {
                std::cout << "entering: awaitClientState" << std::endl;
            }
        };
        struct awaitClientState_Exit
        {
            template <class Event,class FSM,class STATE>
            void operator()(Event const&,FSM&,STATE& )
            {
                std::cout << "leaving: awaitClientState" << std::endl;
            }
        };
        struct awaitClientState_tag {};
        typedef msm::front::euml::func_state<awaitClientState_tag, awaitClientState_Entry, awaitClientState_Exit> awaitClientState;


        // knowingClientState
        struct knowingClientState_tag {};
        typedef msm::front::euml::func_state<knowingClientState_tag> knowingClientState;

        // awaitJoinResponse
        struct awaitJoinResponse_tag {};
        typedef msm::front::euml::func_state<awaitJoinResponse_tag> awaitJoinResponse;

        // joinedSession
        struct joinedSession_tag {};
        typedef msm::front::euml::func_state<joinedSession_tag> joinedSession;

        // error
        struct error_tag {};
        typedef msm::front::euml::func_state<error_tag> error;

        // the initial state of the player SM. Must be defined
        typedef awaitClientState initial_state;

        // transitions


        typedef srv_ s; // makes transition table cleaner

        // Transition table for player
        struct transition_table : mpl::vector<
            //    Start                 Event                Next               Action                     Guard
            //  +------------------- +------------------+----------------------+---------------------------+----------------------+
            Row < awaitClientState   , evClientState    , knowingClientState   , none                      , none                 >,
            Row < knowingClientState , evRequestJoin    , awaitJoinResponse    , none                      , none                 >,
            Row < awaitJoinResponse  , evJoinAccepted   , joinedSession        , none                      , none                 >,
            Row < joinedSession      , evLeave          , knowingClientState   , none                      , none                 >,
            Row < awaitJoinResponse  , evJoinRejected   , error                , none                      , none                 >
        > {};
        // Replaces the default no-transition response.
        template <class FSM,class Event>
        void no_transition(Event const& e, FSM&,int state)
        {
            std::cout << "no transition from state " << state
                << " on event " << typeid(e).name() << std::endl;
        }

    };

    // Pick a back-end
    typedef msm::back::state_machine<srv_> srv;

    //
    // Testing utilities.
    //
    static char const* const state_names[]; // = {  "awaitClientState", "knowingClientState", "awaitJoinResponse", "joinedSession", "error" };
    void pstate(srv const& s)
    {
        std::cout << " -> " << state_names[s.current_state()[0]] << std::endl;
    }

};

} /* namespace muroa */

#endif /* LIBMSTREAM_CSTREAMSRVSM_H_ */
