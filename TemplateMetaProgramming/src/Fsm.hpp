/*
 * Fsm.hpp
 *
 *  Created on: 07.11.2018
 *      Author: CToma
 */

#ifndef FSM_HPP_
#define FSM_HPP_

typedef short Signal;
using namespace std;
#include <iostream>

enum {
	ENTRY_SIG,
	EXIT_SIG,
	SHIFT_DEPRESSED_SIG,
	SHIFT_RELEASED_SIG,
	ANY_KEY_SIG,
};

class Event {
public:
	Event () {
		sig = ANY_KEY_SIG;
	}
	Event (Signal _sig) {
		sig = _sig;
	}
	Signal sig;
	char code;
};

class State {
public:
	explicit State (int _stateID = 0) : stateID(_stateID)
	{
	}

	int stateID;
	virtual void dispatch(Event& f_event) = 0;
};

template <class ParentType>
class TFsm : public ParentType
{
public:
    typedef ParentType parent_type;

    TFsm(State& state) {
    	cout << "State initialized" << endl;
    	_state = &state; //initial state
    }

    State* _state;

    Event entryEvent = Event(ENTRY_SIG);
    Event exitEvent = Event(EXIT_SIG);

    inline void transition(State& f_newState)
	{
    	dispatch(exitEvent);
    	_state = &f_newState;
    	dispatch(entryEvent);
	}

    inline void init()
    {

    }

    void dispatch (Event& event) {
    	cout << "Dispatched called";
    	_state->dispatch(event);
    }

};

// just an empty class
class CVoid
{
  public:
};

typedef TFsm<CVoid> CFsm;
#endif /* FSM_HPP_ */
