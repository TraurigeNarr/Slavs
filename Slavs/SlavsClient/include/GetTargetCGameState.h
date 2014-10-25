#ifndef GetTargetCGameState_h
#define GetTargetCGameState_h

#include "Application.h"
#include "CPlayerController.h"
#include "InputSubscriber.h"

#include <Game/CommandData.h>

#include <OgreSceneQuery.h>

/*
-----------------------------------------------------------------------
This is one of the states of the ClientGameState. It implements
getting position or target (flag target = true -- > returns id of object to extra data).
-----------------------------------------------------------------------
*/
class GetTargetCGameState : public InputSubscriber
{
public:
  GetTargetCGameState();
	GetTargetCGameState(Application* owner, CPlayerController* pController);
	~GetTargetCGameState();

	bool MouseMoved(const OIS::MouseEvent &evt);
	bool MousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
	bool MouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);

	bool ButtonPressed(ButtonID id, void* extraData);
protected:
	void SendTargetData() const;
	void GoToIdleState() const;

	Application*			m_pOwner;
	CPlayerController*		m_pPlayerController;

	CommandData*			m_pCommandData;
	Ogre::RaySceneQuery*	m_pSceneQuery;
};

#endif