/*
 * Keyboard.cpp
 *
 *  Created on: 09.11.2018
 *      Author: ctoma
 */

#include "Keyboard.hpp"
#include "Fsm.hpp"

using namespace std;
#include <iostream>

Keyboard::Keyboard(State& state) : CFsm(init),
		init(*this),
		def(*this),
		shifted(*this) {
}

Keyboard::Keyboard() : CFsm(init),
		init(*this),
		def(*this),
		shifted(*this) {
}


Keyboard::InitState::InitState(Keyboard& keyboard)
    : State(static_cast<int>(0))
    , _keyboard(keyboard)
{
	cout << "InitState was initialized" << endl;
}

Keyboard::DefaultState::DefaultState(Keyboard& keyboard)
    : State(static_cast<int>(1))
    , _keyboard(keyboard)
{
	cout << "DefaultState was initialized" << endl;
}

Keyboard::ShiftedState::ShiftedState(Keyboard& keyboard)
    : State(static_cast<int>(2))
    , _keyboard(keyboard)
{
	cout << "ShiftedState was initialized" << endl;
}

void Keyboard::InitState::dispatch(Event& event)
{
	cout << "Init was called with" << event.sig << endl;
	if (event.sig == EXIT_SIG || event.sig == ENTRY_SIG) {
				cout << "Init was called with exit or entry" << endl;
				return;
	}
	_keyboard.transition(_keyboard.def);
}

void Keyboard::DefaultState::dispatch(Event& event)
{
	cout << "Default was called with" << event.sig << endl;
	if (event.sig == EXIT_SIG || event.sig == ENTRY_SIG) {
			cout << "Default was called with exit or entry" << endl;
			return;
	}
	switch (event.sig) {
			case SHIFT_DEPRESSED_SIG:
			  cout <<"shifted::SHIFT_DEPRESSED_SIG" << endl;
			  _keyboard.transition(_keyboard.shifted);
			  break;
			case ANY_KEY_SIG:
				cout <<"key lowercase" << event.sig << endl;
		   break;
	}
}

void Keyboard::ShiftedState::dispatch(Event& event)
{
	cout << "Shifted was called with" << event.sig << endl;
	if (event.sig == EXIT_SIG || event.sig == ENTRY_SIG) {
		cout << "Shifted was called with exit or entry" << endl;;
		return;
	}
	switch (event.sig) {
		case SHIFT_RELEASED_SIG:
		  cout <<"shifted::SHIFT_RELEASED" << endl;
		  _keyboard.transition(_keyboard.def);
		  break;
		case ANY_KEY_SIG:
			cout <<"key uppercase" << event.sig << endl;
	   break;
	}
}
