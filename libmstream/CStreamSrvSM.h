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
#include <boost/msm/back/tools.hpp>

//msm front-end
#include <boost/msm/front/state_machine_def.hpp>
// functors
//#include <boost/msm/front/functor_row.hpp>
//#include <boost/msm/front/euml/common.hpp>
// for And_ operator
// #include <boost/msm/front/euml/operator.hpp>
// for func_state and func_state_machine
// #include <boost/msm/front/euml/state_grammar.hpp>

namespace msm = boost::msm;
namespace mpl = boost::mpl;
using namespace boost::msm::front;
// for And_ operator
// using namespace boost::msm::front::euml;
#include <typeinfo>
#include <string>
#include <cmds/CmdStream.h>
#include <ISrvSMActions.h>

using namespace muroa;
using namespace std;

namespace muroa {


struct VisitorBase
{
    template <class T>
    void visit_state(T* astate,int i)
    {
        std::cout << "visiting state:" << typeid(*astate).name() << std::endl;
    }
};


// overwrite of the base state (not default)
struct VisitableState
{
    // signature of the accept function
    typedef boost::msm::back::args<void, VisitorBase&> accept_sig;

    // we also want polymorphic states
    virtual ~VisitableState() {}
    // default implementation for states who do not need to be visited
    void accept(VisitorBase&) const {};
    const std::string state_name;
};

struct srv_ : public boost::msm::front::state_machine_def<srv_, VisitableState>
{
	ISrvSMActions* _actions;

	// The list of FSM states

    struct awaitClientState : public boost::msm::front::state<VisitableState>
    {
    	template <class Event,class FSM>
    	void on_entry(Event const&,FSM& ) {
    		std::cout << "entering: awaitClientState" << std::endl;
    	}
    	template <class Event,class FSM>
    	void on_exit(Event const&,FSM& ) {
    	    std::cout << "leaving: awaitClientState" << std::endl;
    	}
    	void accept(VisitorBase&) const     	{
    	    std::cout << "in " << typeid(this).name() << std::endl;
    	};
    };


    // knowingClientState
    struct knowingClientState : public boost::msm::front::state<VisitableState>  {
       	template <class Event, class FSM> void on_entry(Event const& evt, FSM& fsm) {
        		std::cout << "entering: knowingClientState" << std::endl;
        		fsm._actions->reportClientState(&evt);
        }
    	void accept(VisitorBase&) const     	{
    	    std::cout << "in " << typeid(this).name() << std::endl;
    	};
    };

    // awaitJoinResponse
    struct awaitJoinResponse : public boost::msm::front::state<VisitableState>  {
       	template <class Event, class FSM> void on_entry(Event const& evt, FSM& fsm) {
        		std::cout << "entering: awaitJoinResponse" << std::endl;
        		fsm._actions->requestJoin(&evt);
        }
    	void accept(VisitorBase&) const     	{
    	    std::cout << "in " << typeid(this).name() << std::endl;
    	};
    };

    // error
    struct error : public boost::msm::front::state<VisitableState>  {
    	void accept(VisitorBase&) const     	{
    	    std::cout << "in " << typeid(this).name() << std::endl;
    	};
    };


    // joinedSession composite state with inner state machine
    struct joinedSession_ : public boost::msm::front::state_machine_def<joinedSession_, VisitableState>  {
        // inner FSM states

    	void accept(VisitorBase&) const     	{
    	    std::cout << "in " << typeid(this).name() << std::endl;
    	};

    	// definition of awaitSessionState
        struct awaitSessionState : public boost::msm::front::state<VisitableState>
        {
        	template <class Event,class FSM>
        	void on_entry(Event const&,FSM& )
        	{
        		std::cout << "entering: awaitSessionState" << std::endl;
        	}
        	template <class Event,class FSM>
        	void on_exit(Event const&,FSM& )
        	{
        	    std::cout << "leaving: awaitSessionState" << std::endl;
        	}
        	void accept(VisitorBase&) const     	{
        	    std::cout << "in " << typeid(this).name() << std::endl;
        	};
        };


    	// definition of sessionMember
        struct sessionMember : public boost::msm::front::state<VisitableState>
        {
        	template <class Event,class FSM>
        	void on_entry(Event const&,FSM& )
        	{
        		std::cout << "entering: state sessionMember" << std::endl;
        	}
        	template <class Event,class FSM>
        	void on_exit(Event const&,FSM& )
        	{
        	    std::cout << "leaving: state sessionMember" << std::endl;
        	}
        	void accept(VisitorBase&) const     	{
        	    std::cout << "in " << typeid(this).name() << std::endl;
        	};

        };

        // noError
        struct noError : public boost::msm::front::state<VisitableState>  {
        	void accept(VisitorBase&) const     	{
        	    std::cout << "in " << typeid(this).name() << std::endl;
        	};
        };

        // errorExit
        struct errorExit : public boost::msm::front::terminate_state<VisitableState> // errorExit terminates the state machine
        {
        	template <class Event,class FSM>
        	void on_entry(Event const&,FSM& ) {std::cout << "starting: errorExit" << std::endl;}
        	template <class Event,class FSM>
        	void on_exit(Event const&,FSM& ) {std::cout << "finishing: errorExit" << std::endl;}
        	void accept(VisitorBase&) const     	{
        	    std::cout << "in " << typeid(this).name() << std::endl;
        	};
        };

        // the initial state of the player SM. Must be defined
        typedef boost::mpl::vector<awaitSessionState, noError> initial_state;

        // transition actions
        void report_error(const evError& err) {
        };

        void report_timeout(const evTimeout& to) {
        };

        typedef joinedSession_ j; // makes transition table cleaner

        // Transition table for joinedSession
        struct transition_table : boost::mpl::vector<
            //    Start                 Event                Next               Action                     Guard
            //  +------------------- +------------------+----------------------+---------------------------+----------------------+
           _row < awaitSessionState  , evSessionState   , sessionMember                                                           >,
          _irow < sessionMember      , evResetStream                                                                              >,
          _irow < sessionMember      , evSyncStream                                                                               >,
          _irow < sessionMember      , evSetVolume                                                                                >,
            //  +------------------- +------------------+----------------------+---------------------------+----------------------+
          _irow < noError            , evCmdSent                                                                                  >,
			a_row < noError          , evError          , errorExit            , &j::report_error                                 >,
            a_row < noError          , evTimeout        , errorExit            , &j::report_timeout                               >
        > {};
        // Replaces the default no-transition response.
        template <class FSM,class Event>
        void no_transition(Event const& e, FSM&,int state)
        {
            std::cout << " (sub state machine) no transition from state " << state
                << " on event " << typeid(e).name() << std::endl;
        }

    };
    // Pick a back-end for inner state machine
    typedef boost::msm::back::state_machine<joinedSession_> joinedSession;


    // the initial state of the player SM. Must be defined
    typedef awaitClientState initial_state;

    // transitions


    typedef srv_ s; // makes transition table cleaner

    // Transition table for player
    struct transition_table : boost::mpl::vector<
        //    Start                 Event                Next               Action                     Guard
        //  +------------------- +------------------+----------------------+---------------------------+----------------------+
       _row < awaitClientState   , evClientState    , knowingClientState                                                      >,
       _row < knowingClientState , evRequestJoin    , awaitJoinResponse                                                       >,
       _row < awaitJoinResponse  , evJoinAccepted   , joinedSession                                                           >,
       _row < joinedSession      , evLeave          , knowingClientState                                                      >,
       _row < awaitJoinResponse  , evJoinRejected   , error                                                                   >
    > {};

    // Replaces the default no-transition response.
    template <class FSM,class Event>
    void no_transition(Event const& e, FSM&,int state)
    {
        std::cout << "(outer) no transition from state " << state << " on event " << typeid(e).name() << std::endl;
    }

};

class CStreamSrvSM :public boost::msm::back::state_machine<srv_, VisitableState> {
public:
	CStreamSrvSM(ISrvSMActions* actions);
	virtual ~CStreamSrvSM();

	virtual bool onEvent(muroa::CmdStreamBase* ev);

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

#endif /* LIBMSTREAM_CSTREAMSRVSM_H_ */
