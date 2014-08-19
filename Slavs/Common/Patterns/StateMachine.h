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
  T* mp_owner;
  TState mp_current_state;
  TState mp_previous_state;
  TState mp_global_state;

  TState mp_next_state;

private:
  void _ChangeState();

public:
  StateMachine(T* ip_owner);
  virtual ~StateMachine();

  void SetGlobalState(TState ip_global);
  void SetCurrentState(TState ip_current);
  void SetPreviousState(TState ip_previous);

  // sets previous state to current and current to ip_next_state
  void ChangeState(TState ip_next_state);
  void RevertToPreviousState();

  //call this to update the FSM
  void Update(Parameter i_parameter);

  //returns true if the current state's type is equal to the type of the
	//class passed as a parameter. 
  bool IsInState(TState ip_state) const;
  template <typename StateType>
  bool IsInState() const;

  TState GetCurrentState() const
    {
    return mp_current_state;
    }

  bool HandleMessage(const Telegram& i_message);
};

/////////////////////////////////////////////////////////////////////////////////////////
template <typename T, typename Parameter>
StateMachine<T, Parameter>::StateMachine(T* ip_owner)
  : mp_owner(ip_owner)
  , mp_global_state(nullptr)
  , mp_current_state(nullptr)
  , mp_previous_state(nullptr)
  , mp_next_state(nullptr)
{
}

template <typename T, typename Parameter>
StateMachine<T, Parameter>::~StateMachine()
  {
  if (mp_current_state)
    mp_current_state->Exit(mp_owner);
  if (mp_global_state)
    mp_global_state->Exit(mp_owner);
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
  mp_next_state = ip_next_state;
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

  // after update state can change
  if (mp_next_state)
    {
    if (mp_current_state)
      mp_current_state->Exit(mp_owner);
    _ChangeState();
    }

  //same for the current state
  if (nullptr != mp_current_state.get())
    mp_current_state->Execute(mp_owner, i_parameter);  
}

template <typename T, typename Parameter>
void StateMachine<T, Parameter>::_ChangeState()
  {
  if (mp_current_state)
    mp_current_state->Exit(mp_owner);

  SetPreviousState(mp_current_state);

  //change state to the new state
  mp_current_state.reset();
  mp_current_state = mp_next_state;
  mp_next_state = nullptr;

  //call the entry method of the new state
  mp_current_state->Enter(mp_owner);

  }

template <typename T, typename Parameter>
bool StateMachine<T, Parameter>::IsInState(TState ip_state) const
{
  return typeid(*mp_current_state.get()) == typeid(*ip_state.get());
}

template <typename T, typename Parameter>
template <typename StateType>
bool StateMachine<T, Parameter>::IsInState() const
  {
  return typeid(*mp_current_state.get()) == typeid(StateType);
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