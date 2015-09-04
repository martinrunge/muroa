/*
 Copyright (c) 2002-2014 "Martin Runge"

 CStreamClientSM.h is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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

#ifndef LIBMSTREAM_CSTREAMCLIENTSM_H_
#define LIBMSTREAM_CSTREAMCLIENTSM_H_


// msm back-end
#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/back/tools.hpp>


//msm front-end
#include <boost/msm/front/state_machine_def.hpp>
// functors
#include <boost/msm/front/functor_row.hpp>
#include <boost/msm/front/euml/common.hpp>
// for And_ operator
#include <boost/msm/front/euml/operator.hpp>
// for func_state and func_state_machine
#include <boost/msm/front/euml/state_grammar.hpp>

// using namespace boost::msm::front;
// for And_ operator
// using namespace msm::front::euml;
#include <typeinfo>
#include <string>
#include <cmds/CmdStream.h>

#include <IClientSMActions.h>

using namespace muroa;
using namespace std;

namespace muroa {

namespace msm = boost::msm;
namespace mpl = boost::mpl;

struct VisitorBase
{
    template <class T>
    void visit_state(T* astate,int i)
    {
        std::cout << "visiting state:" << astate->state_name << "[" << typeid(*astate).name() << "]" << std::endl;
    }

    void addState(const std::string state_name) {
    	active_states.push_back(state_name);
    }

    void clear() {
    	active_states.clear();
    }

    void printActiveStates() {
    	for(vector<string>::iterator it = active_states.begin(); it != active_states.end(); it++  ) {
    		cout << *it << endl;
    	}
    }
    vector<string> active_states;
};


// overwrite of the base state (not default)
struct VisitableState
{
    // signature of the accept function
    typedef msm::back::args<void, VisitorBase&> accept_sig;

    // we also want polymorphic states
    virtual ~VisitableState() {}
    // default implementation for states who do not need to be visited
    void accept(VisitorBase& vis) const {
    	vis.addState("<state without accept method>");
    };
};

struct clnt_ : public msm::front::state_machine_def<clnt_, VisitableState>
{

	std::string m_ownerSession;
	uint32_t m_open_join_cmd_ID;

	IClientSMActions* _actions;
    // The list of FSM states

    struct awaitReaction : public msm::front::state<VisitableState>
    {
    	template <class Event,class FSM>
    	void on_entry(Event const&,FSM& ) {
    		// sendClientState();
    		std::cout << "entering: awaitReaction" << std::endl;
    	}
    	template <class Event,class FSM>
    	void on_exit(Event const&,FSM& ) {
    	    std::cout << "leaving: awaitClientState" << std::endl;
    	}
    	void accept(VisitorBase& vis) const     	{
        	vis.addState("awaitReaction");
    	};
    };


    // awaitAck
    struct awaitAck : public msm::front::state<VisitableState>  {
    	void accept(VisitorBase& vis) const     	{
        	vis.addState("awaitAck");
    	};
    };

    // sessionMember
    struct sessionMember : public msm::front::state<VisitableState>  {
    	void accept(VisitorBase& vis) const {
    	    vis.addState("sessionMember");
    	};
    };

    // exitState
    struct exitState : public msm::front::terminate_state<VisitableState> // errorExit terminates the state machine
    {
    	template <class Event,class FSM>
    	void on_entry(Event const&,FSM& ) {std::cout << "starting: exitState" << std::endl;}
    	template <class Event,class FSM>
    	void on_exit(Event const&,FSM& ) {std::cout << "finishing: exitState" << std::endl;}
    	void accept(VisitorBase& vis) const     	{
    		vis.addState("exitState");
    	};
    };

    // transition actions
     void sendEvJoinRejected(const evRequestJoin& rj) {
     };

     void sendEvJoinAccepted(const evRequestJoin& rj) {
    	 m_ownerSession = rj.m_session_name;
    	 m_open_join_cmd_ID = rj.m_cmd_id;
     };

     void becomeSessionMember(const evAck& ack) {
     };

     void sendEvLeave(const evLeave& lv) {
     };

     void sendEvLeave(const evTimeout& to) {
     };

     void sendEvError(const evError& err) {
     };

     void sendEvError(const evTimeout& to) {
     };

     void resetStream(const evResetStream& rs) {
     };

     void syncStream(const evSyncStream& ss) {
     };

     void setVolume(const evSetVolume& sv) {

     };

     void sendClientState(const evClientState& cc) {

     };

     // guards
     bool mayJoin(const evRequestJoin& rj) {
    	 if(m_ownerSession.compare("none") == 0 || m_ownerSession.compare( rj.m_session_name ) == 0) {
    		 return true;
    	 }
    	 else {
    		 return false;
    	 }

     }

    // the initial state of the player SM. Must be defined
    typedef awaitReaction initial_state;


    // transitions
    typedef clnt_ c; // makes transition table cleaner

    // Transition table for player
    struct transition_table : mpl::vector<
        //    Start                 Event                Next               Action                     Guard
        //    +--------------------+------------------+----------------------+---------------------------+----------------------+
        a_row < awaitReaction      , evLeave          , exitState            , &c::sendEvLeave           >,
          row < awaitReaction      , evRequestJoin    , exitState            , &c::sendEvJoinRejected    , &c::mayJoin          >,
          row < awaitReaction      , evRequestJoin    , awaitAck             , &c::sendEvJoinAccepted    , &c::mayJoin          >,
        a_row < awaitReaction      , evTimeout        , exitState            , &c::sendEvLeave           >,
        //    +--------------------+------------------+----------------------+---------------------------+----------------------+
        a_row < awaitAck           , evError          , exitState            , &c::sendEvError           >,
        a_row < awaitAck           , evTimeout        , exitState            , &c::sendEvError           >,
        a_row < awaitAck           , evAck            , sessionMember        , &c::becomeSessionMember   >,
        //    +--------------------+------------------+----------------------+---------------------------+----------------------+
        a_irow < sessionMember      , evResetStream                          , &c::resetStream           >,
		a_irow < sessionMember      , evSyncStream                           , &c::syncStream            >,
		a_irow < sessionMember      , evSetVolume                            , &c::setVolume             >,
		a_irow < sessionMember      , evClientState                          , &c::sendClientState       >
    > {};
    // Replaces the default no-transition response.
    template <class FSM,class Event>
    void no_transition(Event const& e, FSM&,int state)
    {
        std::cout << "no transition from state " << state
            << " on event " << typeid(e).name() << std::endl;
    }

};

class CStreamClientSM :public msm::back::state_machine<clnt_, VisitableState> {
public:
	CStreamClientSM(IClientSMActions* actions);
	virtual ~CStreamClientSM();


	void activeStates(VisitorBase& vis);
    void pstate();

protected:
	// states: awaitClientState, knowingClientState, awaitJoinResponse, joinedSession, error
    // front-end: define the FSM structure

    //
    // Testing utilities.
    //
    static char const* const outer_state_names[];
    static char const* const inner_state_names[];
};

} /* namespace muroa */

#endif /* LIBMSTREAM_CSTREAMCLIENTSM_H_ */
