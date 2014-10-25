#pragma once

#include "Telegram.h"

#include <memory>

template <typename StateOwner, typename Param>
class State
{  
public:
	virtual ~State(){}

	//this will execute when the state is entered
	virtual void Enter(StateOwner* ip_owner) = 0;

	//this is the states normal update function
	virtual void Execute(StateOwner* ip_owner, Param i_parameter) = 0;

	//this will execute when the state is exited. (My word, isn't
	//life full of surprises... ;o))
	virtual void Exit(StateOwner* ip_owner) = 0;

	/*this executes if the agent receives a message from
	message dispatcher*/
	virtual bool OnMessage(StateOwner* ip_owner, const Telegram&) { return false; }
};