#ifndef IdleCGameState_h
#define IdleCGameState_h

#include "Application.h"
#include "CPlayerController.h"
#include "InputSubscriber.h"
#include "SelectionBox.h"

#include <OIS.h>
#include <OgrePlaneBoundedVolume.h>
#include <OgreRay.h>
#include <OgreSceneQuery.h>
#include <OgreVector2.h>
#include <OgreVector2.h>

#include <vector>


/*
-----------------------------------------------------------------------
This is one of the states of the ClientGameState. It implements simple
transfer of commands to server and selection of the objects.
-----------------------------------------------------------------------
*/
class IdleCGameState : public InputSubscriber
{
public:
  IdleCGameState();
	IdleCGameState(Application* owner, CPlayerController* pController);
	~IdleCGameState();

	bool	MouseMoved(const OIS::MouseEvent &evt);
	bool	MousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
	bool	MouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);

	bool	ButtonPressed(ButtonID id, void* extraData);
protected:
	void	PerformSelection(const Ogre::Vector2& first, const Ogre::Vector2& second);
	void	ProcessQuery(Ogre::SceneQueryResult& objects);
	//helper function that sends selected ids to server and to screen manager so that it can update GUI in necessary way
	void	SendSelectedIDs(const std::vector<long>& ids);
	void	ClearSelection();

	//selecting box visible
	Ogre::PlaneBoundedVolumeListSceneQuery*	m_VolQuery;
	SelectionBox*							m_pSelectionBox;
	Ogre::Vector2							m_v2Start, m_v2Stop;
	Ogre::RaySceneQuery*					m_pRaySceneQuery;

	std::vector<long>						m_vSelectedObjects;

	Application*							m_pOwner;
	CPlayerController*						m_pPlayerController;
};

#endif