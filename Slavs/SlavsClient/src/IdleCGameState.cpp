#include "IdleCGameState.h"
#include "ScreenManager.h"
#include "ObjectInformation.h"
//states
#include "ClientGameState.h"
#include "GetTargetCGameState.h"
#include "PlaceBuildingCGameState.h"
//client
#include "CCamera.h"
#include "CGameContext.h"
#include "CGameObject.h"
#include "CRendererComponent.h"
#include "OgreFramework.h"
#include "VisualInformation.h"
//CEGUI & OGRE
#include <CEGUI/CEGUI.h>
//common
#include <Game/CommandData.h>
#include <Game\Enumerations.h>
#include <Math\Vector2D.h>
#include <Patterns\Singleton.h>
#include <Utilities\TemplateFunctions.h>
//net
#include <Net.h>
//standard
#include <cassert>

IdleCGameState::IdleCGameState()
{

}

IdleCGameState::IdleCGameState(Application* owner, CPlayerController* pController)
	: m_pOwner(owner), m_pPlayerController(pController)
{
	m_pSelectionBox = new SelectionBox();
	m_pRaySceneQuery = Singleton<OgreFramework>::GetInstancePtr()->m_pSceneManager->createRayQuery(Ogre::Ray());
	m_VolQuery = Singleton<OgreFramework>::GetInstancePtr()->m_pSceneManager->createPlaneBoundedVolumeQuery(Ogre::PlaneBoundedVolumeList());
}

IdleCGameState::~IdleCGameState()
{
	if(NULL != m_pSelectionBox)
		delete m_pSelectionBox;
	if(NULL != m_VolQuery)
		Singleton<OgreFramework>::GetInstancePtr()->m_pSceneManager->destroyQuery(m_VolQuery);
	if(NULL != m_pRaySceneQuery)
		OGRE_DELETE m_pRaySceneQuery;
}

bool IdleCGameState::MouseMoved(const OIS::MouseEvent &evt)
{
	if(m_pSelectionBox->getVisible())
	{
		CEGUI::MouseCursor& mouse = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor();
		m_v2Stop.x = mouse.getPosition().d_x / float(evt.state.width);
		m_v2Stop.y = mouse.getPosition().d_y / float(evt.state.height);

		m_pSelectionBox->setCorners(m_v2Stop, m_v2Stop);
	}
	return false;
}

bool IdleCGameState::MousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	if(id == OIS::MB_Left)
	{
		CEGUI::MouseCursor& mouse = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor();
		m_v2Start.x = mouse.getPosition().d_x / float(evt.state.width);
		m_v2Start.y = mouse.getPosition().d_y / float(evt.state.height);
		m_v2Stop = m_v2Start;
		m_pSelectionBox->clear();
		m_pSelectionBox->setVisible(true);
		return true;
	}
	return false;
}

bool IdleCGameState::MouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	if(id == OIS::MB_Left)
	{
		PerformSelection(m_v2Start, m_v2Stop);
		m_pSelectionBox->setVisible(false);
		return true;
	}
	//if right click rejects choosing position for building
	if(id == OIS::MB_Right)
	{
		ClearSelection();
		return true;
	}
	return false;
}

bool IdleCGameState::ButtonPressed(ButtonID id, void* extraData)
{
	CommandData* cData = Singleton<VisualInformation>::GetInstancePtr()->GetCommandData(id);
	if(NULL != cData)
	{
		if(cData->HasParameter(CDParams_NeedPosition) || cData->HasParameter(CDParams_NeedTarget))
		{
			if(cData->HasParameter(CDParams_CreateBuilding))
			{
				Singleton<ClientGameState>::GetInstancePtr()->SetCurrentState(Singleton<PlaceBuildingCGameState>::GetInstancePtr(), GS_PlacingBuilding);
				Singleton<PlaceBuildingCGameState>::GetInstancePtr()->ButtonPressed(id, cData);
				return false;
			}
			if(cData->HasParameter(CDParams_CharAction))
			{
				Singleton<ClientGameState>::GetInstancePtr()->SetCurrentState(Singleton<GetTargetCGameState>::GetInstancePtr(), GS_GettingTarget);
				Singleton<GetTargetCGameState>::GetInstancePtr()->ButtonPressed(id, cData);
				return false;
			}
		}
		else
		{
			int neededSize = sizeof(PacketType) + cData->NeededSize();
			char *buf = new char[neededSize];
			ToChar(PT_Command, buf, sizeof(PacketType));
			int s = cData->Serialize(buf + sizeof(PacketType), neededSize);
			m_pOwner->GetConnection()->SendPacket(buf, neededSize);
			cData->ClearExtraData();
		}
	}
	return false;
}

void IdleCGameState::PerformSelection(const Ogre::Vector2& first, const Ogre::Vector2& second)
{
	float left = first.x, right = second.x;
	float top = first.y, bottom = second.y;

	if(left > right)
	{
		swap(left, right);
	}
	if(top > bottom)
	{
		swap(top, bottom);
	}

	if((right - left) * (bottom - top) < 0.0001)
	{
		return;
	}

	Ogre::Ray topLeft = Singleton<ClientGameState>::GetInstancePtr()->GetCamera()->GetRay(left, top);
	Ogre::Ray topRight =  Singleton<ClientGameState>::GetInstancePtr()->GetCamera()->GetRay(right, top);
	Ogre::Ray bottomLeft =  Singleton<ClientGameState>::GetInstancePtr()->GetCamera()->GetRay(left, bottom);
	Ogre::Ray bottomRight =  Singleton<ClientGameState>::GetInstancePtr()->GetCamera()->GetRay(right, bottom);

	Ogre::PlaneBoundedVolume vol;

	vol.planes.push_back(Ogre::Plane(topLeft.getPoint(3), topRight.getPoint(3), bottomRight.getPoint(3)));
	vol.planes.push_back(Ogre::Plane(topLeft.getOrigin(), topLeft.getPoint(100), topRight.getPoint(100)));
	vol.planes.push_back(Ogre::Plane(topLeft.getOrigin(), bottomLeft.getPoint(100), topLeft.getPoint(100)));
	vol.planes.push_back(Ogre::Plane(bottomLeft.getOrigin(), bottomRight.getPoint(100), bottomLeft.getPoint(100)));
	vol.planes.push_back(Ogre::Plane(topRight.getOrigin(), topRight.getPoint(100), bottomRight.getPoint(100)));

	Ogre::PlaneBoundedVolumeList volList;
	volList.push_back(vol);
	m_VolQuery->setVolumes(volList);
	m_VolQuery->setQueryMask(QM_Own);

	ProcessQuery(m_VolQuery->execute());
}

void IdleCGameState::ProcessQuery(Ogre::SceneQueryResult& objects)
{
	if(0 == objects.movables.size())
		return;
	Ogre::SceneQueryResultMovableList::iterator iter = objects.movables.begin();
	ClearSelection();

	//get end of the array of the Actors -- does not change
	ObjectsMap::const_iterator endIter = Singleton<ClientGameState>::GetInstancePtr()->GetContext()->GetIteratorEnd();
	ObjectClass oClass = OC_BuildingObjects;

	for(iter; iter != objects.movables.end(); ++iter)
	{
		Ogre::MovableObject *obj = *iter;
		int i = 0;
		//get begin of the array of the Actors
		ObjectsMap::const_iterator objIter =  Singleton<ClientGameState>::GetInstancePtr()->GetContext()->GetIteratorBegin();
		for(objIter; objIter != endIter; ++objIter)
		{
			if(objIter->second->Selected())
				continue;
			CRendererComponent *cRenderer = objIter->second->GetComponent<CRendererComponent>();
			if(Ogre::any_cast<CGameObject*>(&obj->getParentNode()->getUserAny()) && objIter->second == Ogre::any_cast<CGameObject*>(obj->getParentNode()->getUserAny()))
			//if(NULL != cRenderer && obj->getParentSceneNode()->getName() == objIter->second->GetComponent<CRendererComponent>()->GetRootSceneNode().getName())
			{
				if(OC_CharacterObjects <= objIter->second->GetType() && oClass == OC_BuildingObjects)
				{
					ClearSelection();
					oClass = OC_CharacterObjects;
				}
				if(oClass > objIter->second->GetType())
					continue;
				m_vSelectedObjects.push_back(objIter->second->GetID());
				objIter->second->Select();
			}
		}
	}
	if(0 != m_vSelectedObjects.size())
		SendSelectedIDs(m_vSelectedObjects);
}

void IdleCGameState::SendSelectedIDs(const std::vector<long>& ids)
{
	assert(ids.size() != 0 &&
		"IdleCGameState::SingleSelectedIDs");
	long id = ids.at(0);
	Singleton<ScreenManager>::GetInstancePtr()->GetCurrentScreen()->HandleData(ids.size() < 2 ? Cmd_UI_SingleSelection : Cmd_UI_MultipleSelection, &id);
	
	int neededSize = sizeof(PacketType) + ids.size()*sizeof(long);
	char* packet = new char[neededSize];
	ToChar(PT_Selection, packet, sizeof(PacketType));
	int dataOffset = sizeof(PacketType);
	for(unsigned int i = 0; i < ids.size(); ++i)
	{
		ToChar(ids.at(i), packet + dataOffset, sizeof(long));
		dataOffset += sizeof(long);
	}

	m_pOwner->GetConnection()->SendPacket(packet, neededSize);
	delete[] packet;
}

void IdleCGameState::ClearSelection()
{
	Singleton<ScreenManager>::GetInstancePtr()->GetCurrentScreen()->HandleData(Cmd_UI_Deselect, NULL);
	for(unsigned int i = 0; i < m_vSelectedObjects.size(); ++i)
	{
		 Singleton<ClientGameState>::GetInstancePtr()->GetContext()->GetGameObject(m_vSelectedObjects.at(i))->Deselect();
	}
	m_vSelectedObjects.clear();
}