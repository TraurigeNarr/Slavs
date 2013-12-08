#pragma once
#include "State.h"
#include "Telegram.h"

#include <cassert>
#include <memory>

///////////////////////////////////////////////////////////////////////

/*
Purpose: provide simple FSM.
Use: inherit your class from StateMachine. You can add states that has T same as
in StateMachine
*/

template <typename T, typename Parameter>
class StateMachine
{
public:
  typedef std::shared_ptr<State<T, Parameter>> TState;

private:
  std::shared_ptr<T> mp_owner;
  TState mp_current_state;
  TState mp_previous_state;
  TState mp_global_state;

public:
  StateMachine(std::shared_ptr<T> ip_owner);
  virtual ~StateMachine();

  void SetGlobalState(TState ip_global);
  void SetCurrentState(TState ip_current);
  void SetPreviousState(TState ip_previous);

  //sets previous state to current and current to ip_next_state
  void ChangeState(TState ip_next_state);
  void RevertToPreviousState();

  //call this to update the FSM
  void Update(Parameter i_parameter);

  //returns true if the current state's type is equal to the type of the
	//class passed as a parameter. 
  bool IsInState(TState ip_state) const;

  bool HandleMessage(const Telegram& i_message);
};

/////////////////////////////////////////////////////////////////////////////////////////
template <typename T, typename Parameter>
StateMachine<T, Parameter>::StateMachine(std::shared_ptr<T> ip_owner)
  : mp_owner(ip_owner)
{
}

template <typename T, typename Parameter>
StateMachine<T, Parameter>::~StateMachine()
{
}


template <typename T, typename Parameter>
void StateMachine<T, Parameter>::SetGlobalState(TState ip_global)
{
  if(nullptr != mp_global_state.get())
    mp_global_state->Exit(mp_owner);
  mp_global_state = ip_global;
  if(nullptr != mp_global_state.get())
    mp_global_state->Enter(mp_owner);
}

template <typename T, typename Parameter>
void StateMachine<T, Parameter>::SetCurrentState(TState ip_current)
{
  mp_current_state = ip_current;
  if(nullptr != mp_current_state.get())
    mp_current_state->Enter(mp_owner);
}

template <typename T, typename Parameter>
void StateMachine<T, Parameter>::SetPreviousState(TState ip_previous)
{
  mp_previous_state = ip_previous;
}

template <typename T, typename Parameter>
void StateMachine<T, Parameter>::ChangeState(TState ip_next_state)
{
  assert(nullptr != ip_next_state.get() && "<StateMachine::ChangeState>: trying to change to NULL state");

  //keep a record of the previous state
	mp_previous_state = mp_current_state;

	//call the exit method of the existing state
	mp_current_state->Exit(mp_owner);

	//change state to the new state
	mp_current_state = ip_next_state;

	//call the entry method of the new state
	mp_current_state->Enter(mp_owner);
}

template <typename T, typename Parameter>
void StateMachine<T, Parameter>::RevertToPreviousState()
{
  ChangeState(mp_previous_state);
}

template <typename T, typename Parameter>
void StateMachine<T, Parameter>::Update(Parameter i_parameter)

{
  //if a global state exists, call its execute method, else do nothing
  if(nullptr != mp_global_state.get())
    mp_global_state->Execute(mp_owner, i_parameter);

  //same for the current state
  if (nullptr != mp_current_state.get())
    mp_current_state->Execute(mp_owner, i_parameter);
}

template <typename T, typename Parameter>
bool StateMachine<T, Parameter>::IsInState(TState ip_state) const
{
  return typeid(*mp_current_state.get()) == typeid(*ip_state.get());
}

template <typename T, typename Parameter>
bool StateMachine<T, Parameter>::HandleMessage(const Telegram& i_message)
{
   //if a global state exists, call its execute method, else do nothing
	if(nullptr != mp_global_state.get() && mp_global_state->OnMessage(mp_owner, i_message))
    return true;

	//same for the current stat
  if (nullptr != mp_current_state.get() && mp_current_state->OnMessage(mp_owner, i_message))
    return true;

  return false;
}