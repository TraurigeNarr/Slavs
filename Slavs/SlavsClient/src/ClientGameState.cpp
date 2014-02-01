//this state
#include "ClientGameState.h"
//client
#include "Application.h"
#include "CCamera.h"
#include "CGameContext.h"
#include "InputManager.h"
#include "MessageBox.h"
#include "OgreFramework.h"
#include "ScreenManager.h"
#include "VisualInformation.h"
//states
#include "ClientGlobalState.h"
#include "ClientLoadGameState.h"
#include "ClientMenuState.h"
//game states
#include "GetTargetCGameState.h"
#include "IdleCGameState.h"
#include "PlaceBuildingCGameState.h"
//screens
#include "HUD.h"
//common
#include <Game/CommandData.h>
#include <Game/Enumerations.h>
#include <Game/GameObjectState.h>
#include <Game/TimeController.h>
#include <Utilities/TemplateFunctions.h>
//network
#include "Net.h"

//singletons
template<> std::shared_ptr<TimeController> Singleton<TimeController>::mp_singleton = nullptr;
template<> std::shared_ptr<IdleCGameState> Singleton<IdleCGameState>::mp_singleton = nullptr;
template<> std::shared_ptr<PlaceBuildingCGameState> Singleton<PlaceBuildingCGameState>::mp_singleton = nullptr;
template<> std::shared_ptr<GetTargetCGameState> Singleton<GetTargetCGameState>::mp_singleton = nullptr;

//////////////////////////////////////////////////////////////////////////

ClientGameState::ClientGameState(CGameContext *context)
	: m_pContext(context), m_pCurrentState(NULL), m_pPlayerController(NULL)
{
	new Singleton<VisualInformation>(new VisualInformation());
}

ClientGameState::~ClientGameState()
{
	Singleton<VisualInformation>::ReleaseIfValid();
}

void ClientGameState::Enter(Application* ip_owner)
{
	Singleton<OgreFramework>::GetInstancePtr()->m_pLog->logMessage("Enter ClientGameState....");
	Singleton<ClientLoadGameState>::ReleaseIfValid();

	//create player controller
	m_pPlayerController = new CPlayerController(ip_owner);

	//create collections of states for GameState
	new Singleton<IdleCGameState>(new IdleCGameState(ip_owner, m_pPlayerController));
  new Singleton<PlaceBuildingCGameState>(new PlaceBuildingCGameState(ip_owner, m_pPlayerController));
  new Singleton<GetTargetCGameState>(new GetTargetCGameState(ip_owner, m_pPlayerController));

	SetCurrentState(Singleton<IdleCGameState>::GetInstancePtr(), GS_Idle);

	//initialize Time controller
	new Singleton<TimeController>(new TimeController(0.0f));
	Singleton<TimeController>::GetInstancePtr()->AddSubscriber(m_pContext);
  Singleton<InputManager>::GetInstance().AddSubscriber(this);

	ip_owner->FromGame = true;

	m_State = VarWait;

	CreateScene();
}

void ClientGameState::Execute(Application* ip_owner, long i_elapsed_time)
{
	//hold all packets
	unsigned char *packet = new unsigned char[PACKET_SIZE];
	while ( true )
	{
		int bytes_read = ip_owner->GetConnection()->ReceivePacket( packet, (PACKET_SIZE) );

		if ( 0 == bytes_read )
		{
			break;
		}
		HoldPacket(ip_owner, packet, bytes_read);
	}
	delete []packet;
	//update time controller
	Singleton<TimeController>::GetInstancePtr()->Update(i_elapsed_time);
	if(VarGoToMenu == m_State)
	{
		SendCmdToServer(ip_owner->GetConnection());
		ip_owner->GetFSM()->ChangeState(Singleton<ClientMenuState>::GetInstancePtr());
		return;
	}
}

void ClientGameState::Exit(Application* ip_owner)
{
	Singleton<OgreFramework>::GetInstancePtr()->m_pLog->logMessage("Exit ClientGameState....");
	//delete collection of states
  Singleton<IdleCGameState>::ReleaseIfValid();
  Singleton<PlaceBuildingCGameState>::ReleaseIfValid();
  Singleton<GetTargetCGameState>::ReleaseIfValid();
  //delete Time controller
  Singleton<TimeController>::ReleaseIfValid();
	ip_owner->StopConnection();
  Singleton<InputManager>::GetInstance().RemoveSubscriber(this);
  m_pContext->ReleaseContext();
  delete m_pContext;
}

void ClientGameState::SetCurrentState(std::shared_ptr<InputSubscriber> newState, GameStateModes mode)
{
	if(nullptr != newState.get())
	{
		m_pCurrentState = newState;
		Singleton<ScreenManager>::GetInstance().GetCurrentScreen()->HandleData(Cmd_UI_ChangeMode, &mode);
	}
}

void ClientGameState::CreateScene()
{
	BuildGUI();

	SetLight();

	m_pCamera = new CCamera();
	//add camera to time listeners
	Singleton<TimeController>::GetInstance().AddSubscriber(m_pCamera);
}

void ClientGameState::BuildGUI()
{
	Singleton<ScreenManager>::GetInstancePtr()->SetCurrentScreen(new HUD());
}

void ClientGameState::SetLight()
{
	Ogre::Vector3 lightdir(0, -1, 1 );
	lightdir.normalise();

	m_Light = Singleton<OgreFramework>::GetInstancePtr()->m_pSceneManager->createLight("GameLight");
	m_Light->setType(Ogre::Light::LT_DIRECTIONAL);
	m_Light->setDirection(lightdir);
	m_Light->setDiffuseColour(Ogre::ColourValue::White);
	m_Light->setSpecularColour(Ogre::ColourValue(0.4f, 0.4f, 0.4f));

	Singleton<OgreFramework>::GetInstancePtr()->m_pSceneManager->setAmbientLight(Ogre::ColourValue(1,1,1));
}

void ClientGameState::SendCmdToServer(net::Connection *connection)
{
	int packetType = PT_EndGame;
	char *packetToServer = ToChar(packetType);
	connection->SendPacket( packetToServer, sizeof( packetToServer ) );
}

/************************************************************************/
/*                           Event handlers                             */
/************************************************************************/
bool ClientGameState::KeyPressed(const OIS::KeyEvent &keyEventRef)
{
	if(m_pCurrentState->KeyPressed(keyEventRef))
		return true;
	if(OIS::KC_ESCAPE == keyEventRef.key)
	{
		Singleton<ScreenManager>::GetInstance().GetCurrentScreen()->ShowMessageBox("You want to exit? Are you shore?", MBS_OK|MBS_Cancel);
    
		return true;
	}

	return false;
}

bool ClientGameState::KeyReleased(const OIS::KeyEvent &keyEventRef)
{
	if(m_pCurrentState->KeyReleased(keyEventRef))
		return true;
	return false;
}

bool ClientGameState::MouseMoved(const OIS::MouseEvent &evt)
{
	if(m_pCurrentState->MouseMoved(evt))
		return true;
	return false;
}

bool ClientGameState::MousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	if(m_pCurrentState->MousePressed(evt, id))
		return true;
	return false;
}

bool ClientGameState::MouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	if(m_pCurrentState->MouseReleased(evt, id))
		return true;
	return false;
}

bool ClientGameState::ButtonPressed(ButtonID id, void* extraData)
{
	if(m_pCurrentState->ButtonPressed(id))
		return true;
	switch(id)
	{
	case Btn_UI_Menu:
		Singleton<ScreenManager>::GetInstance().GetCurrentScreen()->ShowMessageBox("You want to exit? Are you shore?", MBS_OK|MBS_Cancel);
		return true;
		break;
  case Btn_UI_OK:
    m_State = VarGoToMenu;
    break;
	}
	return true;
}

/************************************************************************/
/*                          Packets                                     */
/************************************************************************/
void ClientGameState::HoldPacket(Application* owner, unsigned char *packet, int size)
{
	PacketType pType = (PacketType)FromChar<int>((char*)packet);
	packet += sizeof(int);
	size -= sizeof(int);
	GameObjectState state;
	switch(pType)
	{
	case PT_GOState:
		state.Deserialize((char*)packet);
		m_pContext->ApplyState(state.lID, state);
		break;
	case PT_ControllerState:
		m_pPlayerController->Deserialize((char*)packet, size);
		break;
	}
}