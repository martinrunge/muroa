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
// #include <boost/msm/front/functor_row.hpp>
// #include <boost/msm/front/euml/common.hpp>
// for And_ operator
// #include <boost/msm/front/euml/operator.hpp>
// for func_state and func_state_machine
// #include <boost/msm/front/euml/state_grammar.hpp>

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

    struct VisitorBase {
        template<class T>
        void visit_state(T *astate, int i) {
            std::cout << "visiting state:" << astate->state_name << "[" << typeid(*astate).name() << "]" << std::endl;
        }

        void addState(const std::string state_name) {
            active_states.push_back(state_name);
        }

        void clear() {
            active_states.clear();
        }

        void printActiveStates() {
            for (vector<string>::iterator it = active_states.begin(); it != active_states.end(); it++) {
                cout << *it << endl;
            }
        }

        vector<string> active_states;
    };


// overwrite of the base state (not default)
    struct VisitableState {
        // signature of the accept function
        typedef msm::back::args<void, VisitorBase &> accept_sig;

        // we also want polymorphic states
        virtual ~VisitableState() {}

        // default implementation for states who do not need to be visited
        void accept(VisitorBase &vis) const {
            vis.addState("<state without accept method>");
        };
    };

    struct clnt_ : public msm::front::state_machine_def<clnt_, VisitableState> {

        // evRequestJoin openJoinReq;

        IClientSMActions *_actions;
        // The list of FSM states

        struct awaitReaction : public msm::front::state<VisitableState> {
            template<class Event, class FSM>
            void on_entry(Event const &evt, FSM &fsm) {
                std::cout << "entering: awaitReaction" << std::endl;
                fsm._actions->sendClientState();
            }

            template<class Event, class FSM>
            void on_exit(Event const &, FSM &) {
                std::cout << "leaving: awaitReaction" << std::endl;
            }

            void accept(VisitorBase &vis) const {
                vis.addState("awaitReaction");
            };
        };


//    // awaitAck
//    struct awaitAck : public msm::front::state<VisitableState>  {
//    	template <class Event,class FSM>
//    	void on_entry(Event const& evt, FSM& fsm) {
//    		std::cout << "entering: awaitAck" << std::endl;
//    		fsm._actions->confirmJoinRequest(evt);
//    	}
//    	void accept(VisitorBase& vis) const     	{
//        	vis.addState("awaitAck");
//    	};
//    };

        // sessionMember
        struct sessionMember : public boost::msm::front::state_machine_def<sessionMember, VisitableState>  {
            template<class Event, class FSM>
            void on_entry(Event const &evt, FSM &fsm) {
                std::cout << "entering: sessionMember" << std::endl;
                fsm._actions->becomeSessionMember(evt);
            }
            template<class Event, class FSM>
            void on_exit(Event const &evt, FSM &fsm) {
                std::cout << "leaving state: sessionMember" << std::endl;
            }

            void accept(VisitorBase &vis) const {
                vis.addState("sessionMember");
            };

        	// definition of mediaConsumer state
            struct mediaConsumer : public boost::msm::front::state<VisitableState>
            {
            	template <class Event,class FSM>
            	void on_entry(Event const&,FSM& )
            	{
            		std::cout << "entering: mediaConsumer" << std::endl;
            	}
            	template <class Event,class FSM>
            	void on_exit(Event const&,FSM& )
            	{
            	    std::cout << "leaving: mediaConsumer" << std::endl;
            	}
            	void accept(VisitorBase&) const     	{
            	    std::cout << "in " << typeid(this).name() << std::endl;
            	};
            };

        	// definition of awaitBecomeProv state
            struct awaitBecomeProv : public boost::msm::front::state<VisitableState>
            {
            	template <class Event,class FSM>
            	void on_entry(Event const&,FSM& )
            	{
            		std::cout << "entering: awaitBecomeProv" << std::endl;
            	}
            	template <class Event,class FSM>
            	void on_exit(Event const&,FSM& )
            	{
            	    std::cout << "leaving: awaitBecomeProv" << std::endl;
            	}
            	void accept(VisitorBase&) const     	{
            	    std::cout << "in " << typeid(this).name() << std::endl;
            	};
            };

        	// definition of mediaProvider state
            struct mediaProvider : public boost::msm::front::state<VisitableState>
            {
            	template <class Event,class FSM>
            	void on_entry(Event const&,FSM& )
            	{
            		std::cout << "entering: mediaProvider" << std::endl;
            	}
            	template <class Event,class FSM>
            	void on_exit(Event const&,FSM& )
            	{
            	    std::cout << "leaving: mediaProvider" << std::endl;
            	}
            	void accept(VisitorBase&) const     	{
            	    std::cout << "in " << typeid(this).name() << std::endl;
            	};
            };

            // the initial state of the sessionMember SM. Must be defined
            typedef mediaConsumer initial_state;

            // Transition table for joinedSession
            //@formatter:off
    		struct transition_table : boost::mpl::vector<
                //    Start                 Event                    Next                   Action                  Guard
                //  +------------------- +------------------+-----------------------+----------------------+---------------------+
               _row < mediaConsumer      , evRequestProv    , awaitBecomeProv                                                    >,
               _row < awaitBecomeProv    , evProvAck        , mediaProvider                                                      >,
               _row < awaitBecomeProv    , evProvRej        , mediaConsumer                                                      >,
               _row < mediaProvider      , evRevokeProv     , mediaConsumer                                                      >
            > {};
    		//@formatter:on

            // Replaces the default no-transition response.
            template <class FSM,class Event>
            void no_transition(Event const& e, FSM&,int state)
            {
                std::cout << " (sub state machine) no transition from state " << state
                    << " on event " << typeid(e).name() << std::endl;
            }

        };

        // exitState
        struct exitState : public msm::front::terminate_state<VisitableState> // errorExit terminates the state machine
        {
            template<class Event, class FSM>
            void on_entry(Event const &evt, FSM &fsm) {
                std::cout << "starting: exitState" << std::endl;
            }

            template<class Event, class FSM>
            void on_exit(Event const &, FSM &) { std::cout << "finishing: exitState" << std::endl; }

            void accept(VisitorBase &vis) const {
                vis.addState("exitState");
            };
        };

        // transition actions
        void sendEvJoinRejected(const evRequestJoin &rj) {
            _actions->sendRejectJoin(rj);
        };

        void sendEvJoinAccepted(const evRequestJoin &rj) {
            _actions->confirmJoinRequest(rj);
        };

        void becomeSessionMember(const evAck &ack) {
        };

        void leaveSession(const evRequestLeave &lv) {
            _actions->leaveSession(lv);
        };

        void sendEvLeave(const evTimeout &to) {
        };


        void sendEvSessionError(const evSessionError &err) {
            _actions->sendEvSessionError(err);
        };

        void sendEvError(const evError &err) {
            _actions->sendEvError(err);
        };

        void sendEvError(const evTimeout &to) {
        };

        void resetStream(const evResetStream &rs) {
            _actions->onResetStream(rs);
        };

        void syncStream(const evSyncStream &ss) {
            _actions->onSyncInfo(ss);
        };

        void setVolume(const evSetVolume &sv) {
            _actions->onSetVolume(sv);
        };

        void sendEvVolume(const evVolume &evv) {
            _actions->sendEvVolume(evv);
        };

        void sendClientState(const evClientState &cs) {
            _actions->sendClientState();
        };

        void sendClientState(const evRequestClientState &rcs) {
            _actions->sendClientState();
        };

        void sendSessionState(const evRequestSessionState &rss) {
            _actions->sendSessionState(rss);
        };

        void onGetSMState(const evGetSMState &gs) {
            _actions->onGetSMState(gs);
        };

        void sendSMState(const evSMState &gs) {
            _actions->sendEvt(gs);
        };

        void onBecomeMediaProvider(const evBecomeMediaProvider &bmp) {
            _actions->onBecomeMediaProvider(bmp);
        };

        void onRevokeMediaProvider(const evRevokeMediaProvider &rmp) {
            _actions->onRevokeMediaProvider(rmp);
        };

        // guards
        bool mayJoin(const evRequestJoin &rj) {
            bool retval = _actions->mayJoinSession(rj);
            return retval;
        }

        bool mayNotJoin(const evRequestJoin &rj) {
            return !mayJoin(rj);
        }


        // the initial state of the player SM. Must be defined
        typedef awaitReaction initial_state;


        // transitions
        typedef clnt_ c; // makes transition table cleaner

        // Transition table for player
        //@formatter:off
        struct transition_table : mpl::vector<
                //    Start                 Event                Next               Action                     Guard
                //    +--------------------+------------------+----------------------+---------------------------+----------------------+
                a_row < awaitReaction      , evError          , exitState            , &c::sendEvError                                  >,
                  row < awaitReaction      , evRequestJoin    , awaitReaction        , &c::sendEvJoinRejected    , &c::mayNotJoin       >,
                  row < awaitReaction      , evRequestJoin    , sessionMember        , &c::sendEvJoinAccepted    , &c::mayJoin          >,
               a_irow < awaitReaction      , evRequestClientState                    , &c::sendClientState                              >,
               a_irow < awaitReaction      , evGetSMState                            , &c::onGetSMState                                 >,
               a_irow < awaitReaction      , evBecomeMediaProvider                   , &c::onBecomeMediaProvider                        >,
               a_irow < awaitReaction      , evRevokeMediaProvider                   , &c::onRevokeMediaProvider                        >,
                //    +--------------------+------------------+----------------------+---------------------------+----------------------+
               a_irow < sessionMember      , evResetStream                           , &c::resetStream                                  >,
               a_irow < sessionMember      , evSyncStream                            , &c::syncStream                                   >,
               a_irow < sessionMember      , evSetVolume                             , &c::setVolume                                    >,
               a_irow < sessionMember      , evVolume                                , &c::sendEvVolume                                 >,
               a_irow < sessionMember      , evRequestSessionState                   , &c::sendSessionState                             >,
               a_irow < sessionMember      , evSessionError                          , &c::sendEvSessionError                           >,
                a_row < sessionMember      , evRequestLeave   , awaitReaction        , &c::leaveSession                                 >,
                a_row < sessionMember      , evError          , exitState            , &c::sendEvError                                  >
        > {};
        //@formatter:on

        // Replaces the default no-transition response.
        template<class FSM, class Event>
        void no_transition(Event const &e, FSM &, int state) {
            std::cout << "no transition from state " << state
                      << " on event " << typeid(e).name() << std::endl;
        }

    };

    class CStreamClientSM : public msm::back::state_machine<clnt_, VisitableState> {
    public:
        CStreamClientSM(IClientSMActions *actions);

        virtual ~CStreamClientSM();

        bool onEvent(muroa::CmdStreamBase *ev);

        void activeStates(VisitorBase &vis);

        void pstate();

    protected:
        // states: awaitClientState, knowingClientState, awaitJoinResponse, joinedSession, error
        // front-end: define the FSM structure

        //
        // Testing utilities.
        //
        static char const *const outer_state_names[];
        static char const *const inner_state_names[];
    };

} /* namespace muroa */

#endif /* LIBMSTREAM_CSTREAMCLIENTSM_H_ */
