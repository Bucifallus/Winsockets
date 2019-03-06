/*
 * Fsm.hpp
 *
 *  Created on: 07.11.2018
 *      Author: CToma
 */

#ifndef Keyboard_HPP_
#define Keyboard_HPP_

#include "Fsm.hpp"

class Keyboard : public CFsm {
public:
	Keyboard();
	Keyboard(State& state);
	class InitState : public State {
	public:
		InitState(Keyboard& _keyboard);
		virtual void dispatch(Event& f_event);

		Keyboard& _keyboard;
	};
	InitState init;

	class DefaultState : public State {
	public:
		DefaultState(Keyboard& _keyboard);
		virtual void dispatch(Event& f_event);

		Keyboard& _keyboard;
		};
	DefaultState def;

	class ShiftedState : public State {
		public:
		ShiftedState(Keyboard& _keyboard);
		virtual void dispatch(Event& f_event);

		Keyboard& _keyboard;
		};
	ShiftedState shifted;

};

#endif /* Keyboard_HPP_ */
