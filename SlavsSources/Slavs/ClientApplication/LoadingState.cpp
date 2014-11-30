#include "stdafx.h"

#include "LoadingState.h"
#include "LoadingStages.h"
#include "LoadStateMessageProvider.h"
#include "ClientGameContext.h"
#include "PlayerController.h"

#include "Application.h"
#include "GameState.h"

#include "ScreenManager.h"
#include "LoadingScreen.h"

#include "UISettings.h"

#include <Common/Patterns/StateMachine.h>

#include <Network/PacketProvider.h>
#include <Network/include/Net.h>

#include <exception>

/////////////////////////////////////////////////////////////////////////

namespace ClientStates
  {

  LoadingState::LoadingState (Application& i_application, const net::Address& i_connection_address)
    : ApplicationStateBase(i_application)
    , m_connection_address(i_connection_address)
    {    }

  LoadingState::~LoadingState()
    {    }

  net::Address LoadingState::_GetClientAddress() const
    {
    return net::Address(127,0,0,1,net::ClientPort);
    }

  void LoadingState::Enter(Application* ip_application)
    {
    // set scene manager
    auto& ogre_framework = m_application.GetOgreFramework();
    Ogre::SceneManagerEnumerator::MetaDataIterator scene_mgr_it = ogre_framework.GetRoot()->getSceneManagerMetaDataIterator();
    for ( ; scene_mgr_it.current() != scene_mgr_it.end(); scene_mgr_it.moveNext())
      {
      if (scene_mgr_it.peekNext()->typeName == "OctreeSceneManager")
        break;
      }

    if (scene_mgr_it.current() != scene_mgr_it.end())
      ogre_framework.CreateSceneManager((*scene_mgr_it.current())->sceneTypeMask, "GameSceneManager");
    else
      ogre_framework.CreateSceneManager(Ogre::ST_GENERIC, "GameSceneManager");

    // set ogre settings
    Ogre::Camera* p_camera = m_application.GetOgreFramework().CreateCamera("LoadingCamera");
    m_application.GetOgreFramework().GetViewport()->setCamera(p_camera);

    // set screen
    using namespace UI;
    ScreenManager& screen_manager = ip_application->GetScreenManager();
    screen_manager.SetCurrentScreen(std::unique_ptr<Screen>(new LoadingScreen(screen_manager)));

    // set network
    //try to initialize sockets
    if(!net::InitializeSockets())
      {
      m_application.GetStateMachine().RevertToPreviousState();
      return;
      }

    mp_connection.reset(new net::Connection(net::ProtocolId, net::TimeOut));
    if (!mp_connection->Start(net::ClientPort))
      return;
    
    mp_packet_provider.reset(new Network::PacketProvicer(*mp_connection));

    mp_message_provider.reset(new LoadStateMessageProvider(*this));

    mp_context.reset(new ClientGameContext("First map", m_application.GetOgreFramework()));
    mp_context->RegisterController( std::unique_ptr<IController>( new PlayerController(_GetClientAddress().GetAddress(), *mp_context) ) );

    screen_manager.GetCurrentScreen()->SetMessageProvider(mp_message_provider.get());

    mp_state_machine.reset(new StateMachine<LoadingState, long>(this));
    mp_state_machine->SetCurrentState(std::make_shared<LoadingStages::ConnectionState>(*mp_packet_provider));
    }

  void LoadingState::Execute(Application* ip_application, long i_elapsed_time)
    {
    try
      {
      if (!mp_state_machine)
        throw std::runtime_error("Connection bad");

      mp_state_machine->Update(i_elapsed_time);
      if (mp_state_machine->IsInState<LoadingStages::ResultsState>())
        {
        if (static_cast<LoadingStages::ResultsState*>(mp_state_machine->GetCurrentState().get())->GetResult() == LoadingStages::LoadingResult::LR_FAILED)
          throw std::runtime_error("Failed result");
        else
          m_application.GetStateMachine().ChangeState(std::make_shared<GameState>(m_application, std::move(mp_context), std::move(mp_packet_provider), std::move(mp_connection)));
        }
      }
    catch (std::runtime_error&)
      {
      mp_connection.reset();
      m_application.GetStateMachine().RevertToPreviousState();
      }
    }

  void LoadingState::Exit(Application* ip_application)
    {
    mp_state_machine.reset();
    mp_packet_provider.reset();

    mp_message_provider.reset();
    ip_application->GetScreenManager().GetCurrentScreen()->SetMessageProvider(nullptr);
    }

  } // ClientStates