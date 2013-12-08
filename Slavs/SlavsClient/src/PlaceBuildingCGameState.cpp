//this
#include "PlaceBuildingCGameState.h"
//client
#include "CCamera.h"
#include "CGameContext.h"
#include "OgreFramework.h"
#include "VisualInformation.h"
//states
#include "ClientGameState.h"
#include "IdleCGameState.h"
//CEGUI & OGRE
#include <CEGUI\CEGUI.h>
//common
#include <Game\IGameObject.h>
#include <Game\Enumerations.h>
#include <Math\Vector2D.h>
#include <Utilities\TemplateFunctions.h>
#include <Utilities\XmlUtilities.h>
//net
#include <Net.h>


PlaceBuildingCGameState::PlaceBuildingCGameState()
{

}

PlaceBuildingCGameState::PlaceBuildingCGameState(Application* owner, CPlayerController* pController)
	: m_pOwner(owner), m_pNode(NULL), m_pPlayerController(pController)
{
	CreatePlane();
}

PlaceBuildingCGameState::~PlaceBuildingCGameState()
{
//	Singleton<OgreFramework>::GetInstancePtr()->m_pSceneManager->destroySceneNode(m_pNode);
	if(!CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().isVisible())
		CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().show();
	m_pNode = NULL;
}

bool PlaceBuildingCGameState::MouseMoved(const OIS::MouseEvent &evt)
{
	CEGUI::Vector2f p = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition();
	Ogre::Vector3 vec = Singleton<ClientGameState>::GetInstancePtr()->GetCamera()->GetWorldPoint(p.d_x, p.d_y);
	m_pNode->setPosition(vec.x, 50, vec.z);
	return false;
}

bool PlaceBuildingCGameState::MousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	return false;
}

bool PlaceBuildingCGameState::MouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	//if right click rejects choosing position for building
	if(id == OIS::MB_Right)
	{
		GoToIdleState();
		return true;
	}
	CEGUI::Vector2f p = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition();
	Ogre::Vector3 vec = Singleton<ClientGameState>::GetInstancePtr()->GetCamera()->GetWorldPoint(p.d_x, p.d_y);
	m_pCommandData->SetExtraData(new Vector2D(vec.x, vec.z));
	SendPosition();
	return true;
}

bool PlaceBuildingCGameState::ButtonPressed(ButtonID id, void* extraData)
{
	if(NULL != extraData)
		m_pCommandData = static_cast<CommandData*>(extraData);
	else
		m_pCommandData = Singleton<VisualInformation>::GetInstancePtr()->GetCommandData(id);

	if(NULL == m_pCommandData)
	{
		Singleton<ClientGameState>::GetInstancePtr()->SetCurrentState(Singleton<IdleCGameState>::GetInstancePtr(), GS_Idle);
		return false;
	}
	GetParameters();
	m_pNode->setVisible(true);
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setVisible(false);
	return true;
}

void PlaceBuildingCGameState::GetParameters()
{
	ObjectType oType = IGameObject::GetGameObjectType(m_pCommandData->GetCommand());
	const TiXmlElement* configElem = Singleton<ClientGameState>::GetInstancePtr()->GetContext()->GetConfigElement(oType);
	if(NULL == configElem)
	{
		GoToIdleState();
		return;
	}
	int size = XmlUtilities::GetIntAttribute(configElem, "size", 0);
	if(0 == size)
		GoToIdleState();
	m_pNode->setScale(size, 0, size);
}

void PlaceBuildingCGameState::SendPosition() const
{
	int neededSize = sizeof(PacketType) + m_pCommandData->NeededSize();
	char *buf = new char[neededSize];
	ToChar(PT_Command, buf, sizeof(PacketType));
	int s = m_pCommandData->Serialize(buf + sizeof(PacketType), neededSize);
	m_pOwner->GetConnection()->SendPacket(buf, neededSize);
	delete[] buf;
	GoToIdleState();
}

void PlaceBuildingCGameState::CreatePlane()
{
	Ogre::MeshManager::getSingleton().createPlane("PB_Plane", "Environment",
		Ogre::Plane(Ogre::Vector3::UNIT_Y, 0), 1, 1, 1, 1, true, 1, 1, 1, Ogre::Vector3::UNIT_Z);
	Ogre::Entity *entity;

	entity = Singleton<OgreFramework>::GetInstancePtr()->m_pSceneManager->createEntity("PB_Plane");
	m_pNode = Singleton<OgreFramework>::GetInstancePtr()->m_pSceneManager->getRootSceneNode()->createChildSceneNode();
	m_pNode->attachObject(entity);
	m_pNode->setPosition(Ogre::Vector3::ZERO);

	entity->setMaterialName("PB_Material", "Environment");
	entity->setCastShadows(false);
	entity->setVisible(false);
	
	m_pNode->setScale(1000, 1000, 1000);
}

void PlaceBuildingCGameState::GoToIdleState() const
{
	m_pNode->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setVisible(true);
	m_pCommandData->ClearExtraData();
	Singleton<ClientGameState>::GetInstancePtr()->SetCurrentState(Singleton<IdleCGameState>::GetInstancePtr(), GS_Idle);
	return;
}