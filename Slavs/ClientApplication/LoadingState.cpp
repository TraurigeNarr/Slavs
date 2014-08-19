#include "stdafx.h"

#include "LoadingState.h"
#include "LoadingStages.h"

#include "Application.h"

#include "ScreenManager.h"
#include "LoadingScreen.h"

#include "PacketProvider.h"

#include <Common/Patterns/StateMachine.h>

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

  void LoadingState::Enter(Application* ip_application)
    {
    // set scene manager
    m_application.GetOgreFramework().CreateSceneManager(Ogre::ST_GENERIC, "LoadState_SceneManager");//->setAmbientLight(Ogre::ColourValue(0.5f, 0.3f, 0.3f));

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
          throw std::runtime_error("Bad");
        else
          throw std::runtime_error("Good");
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
    }

  } // ClientStates