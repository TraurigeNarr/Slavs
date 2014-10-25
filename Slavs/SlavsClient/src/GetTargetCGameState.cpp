#include "GetTargetCGameState.h"
//client
#include "CCamera.h"
#include "CGameContext.h"
#include "CGameObject.h"
#include "VisualInformation.h"
//states
#include "ClientGameState.h"
#include "IdleCGameState.h"
//CEGUI & OGRE
#include <CEGUI\CEGUI.h>
#include "OgreFramework.h"
//common
#include <Game\Enumerations.h>
#include <Math\Vector2D.h>
#include <Patterns\Singleton.h>
#include <Utilities\TemplateFunctions.h>
#include <Utilities\XmlUtilities.h>
//net
#include <Net.h>



GetTargetCGameState::GetTargetCGameState()
{

}

GetTargetCGameState::GetTargetCGameState(Application* owner, CPlayerController* pController)
	: m_pOwner(owner), m_pPlayerController(pController)
{
	m_pSceneQuery = Singleton<OgreFramework>::GetInstancePtr()->m_pSceneManager->createRayQuery(Ogre::Ray());
}

GetTargetCGameState::~GetTargetCGameState()
{
	if(NULL != m_pSceneQuery)
		Singleton<OgreFramework>::GetInstancePtr()->m_pSceneManager->destroyQuery(m_pSceneQuery);
}

bool GetTargetCGameState::MouseMoved(const OIS::MouseEvent &evt)
{
	return false;
}

bool GetTargetCGameState::MousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	return false;
}

bool GetTargetCGameState::MouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	//if right click rejects choosing position for building
	if(id == OIS::MB_Right)
	{
		GoToIdleState();
		return true;
	}
	

	CEGUI::Vector2f p = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition();
	if(m_pCommandData->HasParameter(CDParams_NeedPosition))
	{
		Ogre::Vector3 vec = Singleton<ClientGameState>::GetInstancePtr()->GetCamera()->GetWorldPoint(p.d_x, p.d_y);
		m_pCommandData->SetExtraData(new Vector2D(vec.x, vec.z));
		SendTargetData();
		return true;
	}
	if(m_pCommandData->HasParameter(CDParams_NeedTarget))
	{
		m_pSceneQuery->setQueryMask(QM_All);
		m_pSceneQuery->setRay(Singleton<ClientGameState>::GetInstancePtr()->GetCamera()->GetRay(p.d_x / evt.state.width, p.d_y / evt.state.height));
		IGameObject* temp = Singleton<ClientGameState>::GetInstancePtr()->GetContext()->GetObjectByRay(m_pSceneQuery->execute());
		if(NULL == temp)
		{
			GoToIdleState();
			return true;
		}
		m_pCommandData->SetExtraData(new long(temp->GetID()));
		SendTargetData();
		return true;
	}
	
	return true;
}

bool GetTargetCGameState::ButtonPressed(ButtonID id, void* extraData)
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
	return true;
}

void GetTargetCGameState::SendTargetData() const
{
	int neededSize = sizeof(PacketType) + m_pCommandData->NeededSize();
	char *buf = new char[neededSize];
	ToChar(PT_Command, buf, sizeof(PacketType));
	int s = m_pCommandData->Serialize(buf + sizeof(PacketType), neededSize);
	m_pOwner->GetConnection()->SendPacket(buf, neededSize);
	delete[] buf;
	GoToIdleState();
}

void GetTargetCGameState::GoToIdleState() const
{
	Singleton<ClientGameState>::GetInstancePtr()->SetCurrentState(Singleton<IdleCGameState>::GetInstancePtr(), GS_Idle);
	return;
}