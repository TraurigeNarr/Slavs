#ifndef GameState_h
#define GameState_h

#include "AppState.h"
#include "CPlayerController.h"
#include "ClientEnums.h"

#include <OgreVector2.h>

#include <memory>

namespace net
{
	class Connection;
}

#include "CGameContext.h"
//class CGameContext;
class CCamera;

class ClientGameState : public AppState
{
public:
  //need for Singleton class
  ClientGameState(){}
	ClientGameState(std::unique_ptr<CGameContext> i_context);
	~ClientGameState();

  virtual void Enter(Application* ip_owner) override;
  virtual void Execute(Application* ip_owner, long i_elapsed_time) override;
  virtual void Exit(Application* ip_owner) override;

	void			SetCurrentState(std::shared_ptr<InputSubscriber> newState, GameStateModes mode);
	CGameContext*	GetContext() const { return m_pContext.get(); }
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
	std::unique_ptr<CGameContext>		m_pContext;
	Ogre::Light*		m_Light;
	//GameState states represents by collection of Input subscribers. Each state
	//has it`s own logic of processing input. The first state is Idle
	std::shared_ptr<InputSubscriber>	m_pCurrentState;
	CPlayerController*	m_pPlayerController;
};


#endif