#ifndef GameState_h
#define GameState_h

#include "AppState.h"
#include "CPlayerController.h"
#include "ClientEnums.h"

#include <OgreVector2.h>

namespace net
{
	class Connection;
}

class CGameContext;
class CCamera;

class ClientGameState : public AppState
{
public:
  //need for Singleton class
  ClientGameState(){}
	ClientGameState(CGameContext *context);
	~ClientGameState();

  virtual void Enter(std::shared_ptr<Application> ip_owner) override;
  virtual void Execute(std::shared_ptr<Application> ip_owner, long i_elapsed_time) override;
  virtual void Exit(std::shared_ptr<Application> ip_owner) override;

	void			SetCurrentState(std::shared_ptr<InputSubscriber> newState, GameStateModes mode);
	CGameContext*	GetContext() const { return m_pContext; }
	CCamera*		GetCamera() const { return m_pCamera; }

	bool			KeyPressed(const OIS::KeyEvent &keyEventRef);
	bool			KeyReleased(const OIS::KeyEvent &keyEventRef);

	bool			MouseMoved(const OIS::MouseEvent &evt);
	bool			MousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
	bool			MouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);

	bool			ButtonPressed(ButtonID id, void* extraData);
protected:
	void			HoldPacket(Application* owner, unsigned char *packet, int size);
	void			CreateScene();
	void			BuildGUI();
	void			SetLight();

	void			SendCmdToServer(net::Connection *connection);

	CCamera*			m_pCamera;
	CGameContext*		m_pContext;
	Ogre::Light*		m_Light;
	//GameState states represents by collection of Input subscribers. Each state
	//has it`s own logic of processing input. The first state is Idle
	std::shared_ptr<InputSubscriber>	m_pCurrentState;
	CPlayerController*	m_pPlayerController;
};


#endif