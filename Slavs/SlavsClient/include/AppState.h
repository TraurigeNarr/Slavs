#ifndef AppState_h
#define AppState_h
#include "ClientStateVariable.h"
#include "InputSubscriber.h"

#include <OGRE/OgreFrameListener.h>

#include <Patterns/State.h>

class Application;

class AppState : public State<Application, long>
               , public InputSubscriber
{
protected:
  StateVariables m_State;
public:
  //writes itself to listeners of InputManager
  AppState();
  //removes from listeners
  virtual ~AppState();

	//this will execute when the state is entered
	virtual void Enter(std::shared_ptr<Application> ip_owner) = 0;

	//this is the states normal update function
	virtual void Execute(std::shared_ptr<Application> ip_owner, long i_elapsed_time) = 0;

	//this will execute when the state is exited. (My word, isn't
	//life full of surprises... ;o))
	virtual void Exit(std::shared_ptr<Application> ip_owner) = 0;

	/*this executes if the agent receives a message from
	message dispatcher*/
	virtual bool OnMessage(std::shared_ptr<Application> ip_owner, const Telegram&) { return false; }
};

#endif