#ifndef PlaceBuildingCGameState_h
#define PlaceBuildingCGameState_h

#include "Application.h"
#include "CPlayerController.h"
#include "InputSubscriber.h"
#include "SelectionBox.h"

#include <Game/CommandData.h>

#include <OIS.h>
#include <OgreSceneNode.h>

/*
-----------------------------------------------------------------------
This is one of the states of the ClientGameState. It implements
selecting position for building.
-----------------------------------------------------------------------
*/

class PlaceBuildingCGameState : public InputSubscriber
{
public:
  PlaceBuildingCGameState();
	PlaceBuildingCGameState(Application* owner, CPlayerController* pController);
	~PlaceBuildingCGameState();

	bool MouseMoved(const OIS::MouseEvent &evt);
	bool MousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
	bool MouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);

	bool ButtonPressed(ButtonID id, void* extraData);
protected:
	void GetParameters();
	void SendPosition() const;
	void CreatePlane();
	void GoToIdleState() const;

	Application*		m_pOwner;
	CPlayerController*	m_pPlayerController;

	CommandData*		m_pCommandData;
	Ogre::SceneNode*	m_pNode;
};

#endif