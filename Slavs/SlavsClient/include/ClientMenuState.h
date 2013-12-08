#ifndef MenuState_h 
#define MenuState_h

#include "AppState.h"

class ClientMenuState : public AppState
{
public:
  ClientMenuState();
  ~ClientMenuState();

  virtual void Enter(std::shared_ptr<Application> ip_owner) override;
  virtual void Execute(std::shared_ptr<Application> ip_owner, long i_elapsed_time) override;
  virtual void Exit(std::shared_ptr<Application> ip_owner) override;

	bool KeyPressed(const OIS::KeyEvent &keyEventRef);
	bool KeyReleased(const OIS::KeyEvent &keyEventRef);

	bool MouseMoved(const OIS::MouseEvent &evt);
	bool MousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
	bool MouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);

	bool ButtonPressed(ButtonID id, void* extraData);
protected:
	void CreateScene();
	void BuildGUI();
};

#endif