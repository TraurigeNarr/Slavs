//server
#include "ServerMain.h"
#include "PluginSystem\DllManager.h"
#include "PluginSystem\Plugin.h"

#include "Game\GameContext.h"
#include "Game\GameObject.h"

#include "LoadGameState.h"

#include <locale>
#include <codecvt>

//states
#include "ServerGameState.h"
#include "ServerGlobalState.h"
#include "ServerLoadGameState.h"
#include "ServerWaitState.h"

//common
#include <Patterns/Singleton.h>
#include <Utilities/FileUtilities.h>
#include <Utilities/XmlUtilities.h>
//net
#include <Net.h>
//standard
#include <iostream>
#include <ostream>
#include <cassert>
//boost
#include <boost/lexical_cast.hpp>

#if defined(_WIN32)
#include <windows.h>
#include <strsafe.h>
#endif

template<> std::shared_ptr<GlobalServerState> Singleton<GlobalServerState>::mp_singleton = nullptr;
template<> std::shared_ptr<WaitState> Singleton<WaitState>::mp_singleton = nullptr;
template<> std::shared_ptr<ServerLoadGameState> Singleton<ServerLoadGameState>::mp_singleton = nullptr;
template<> std::shared_ptr<ServerGameState> Singleton<ServerGameState>::mp_singleton = nullptr;

std::unique_ptr<ServerMain> ServerMain::mh_instance= nullptr;

ServerMain::ServerMain()
  : mh_server_connection(nullptr)
  , m_bWorking(false)
  , mh_dll_manager(nullptr)
  , mh_state_machine(nullptr)
  {
  assert (mh_instance.get() == nullptr);
  mh_instance.reset(this);
  }

ServerMain::~ServerMain()
  {
  }

bool ServerMain::Initialize()
{
  FromGame = false;
  m_pFSM = new StateMachine<ServerMain, long>(this);
	//initialize sockets
	if ( !net::InitializeSockets() )
	{
		printf( "failed to initialize sockets\n" );
		return false;
	}

	mh_server_connection.reset(new net::Connection(net::ProtocolId, net::TimeOut));
	if(!mh_server_connection->Start(net::ServerPort))
		return false;

  mh_server_connection->Listen();
	m_bWorking = true;
	
	//TODO: initialize list of maps

	new Singleton<WaitState>(new WaitState());
 	new Singleton<GlobalServerState>(new GlobalServerState());
 	m_pFSM->SetGlobalState(Singleton<GlobalServerState>::GetInstancePtr());
	m_pFSM->SetCurrentState(Singleton<WaitState>::GetInstancePtr());
	return true;
}

void ServerMain::Shutdown()
  {
  // release state machine
  mh_state_machine.reset(nullptr);
  // release states
	Singleton<WaitState>::ReleaseIfValid();
  Singleton<GlobalServerState>::ReleaseIfValid();
  // release libraries
  mh_dll_manager.reset();
  // free sockets
  net::ShutdownSockets();
  }

void ServerMain::Update(long elapsedTime)
  {
  mh_state_machine->Update(elapsedTime);
  }

net::Connection* ServerMain::GetConnection () const
  {
  return mh_server_connection.get();
  }

//////////////////////////////////////////////////////////////////////////

namespace
  {

  int _GetPort(const std::string& i_port)
    {
    int port = 0;
    try
      {
      port = boost::lexical_cast<int>(i_port);
      }
    catch(const boost::bad_lexical_cast &)
      {
      port = net::ServerPort;
      assert (!"Port format is incorrect");
      }
    return port;
    }

  } // namespace

ServerMain& ServerMain::GetInstance()
  {
  assert (mh_instance.get());
  return *mh_instance.get();
  }

MetaFactory& ServerMain::GetMetaFactory()
  {
  return m_meta_factory;
  }

DllManager& ServerMain::GetDllManager()
  {
  return *mh_dll_manager.get();
  }

void ServerMain::RegisterPlugin(Plugin* ip_plugin)
  {
  m_plugins.insert(ip_plugin);
  ip_plugin->Install();
  try
    {
    ip_plugin->Initialize();
    }
  catch (std::invalid_argument&)
    {
    std::cout << ip_plugin->GetName() << " can not be initialized." << std::endl;
    m_plugins.erase(ip_plugin);
    ip_plugin->Uninstall();
    }
  }

void ServerMain::UnregisterPlugin(Plugin* ip_plugin)
  {
  m_plugins.erase(ip_plugin);
  ip_plugin->Release();
  ip_plugin->Uninstall();
  }

ServerMain::TServerFSM& ServerMain::GetStateMachine()
  {
  return *mh_state_machine;
  }

bool ServerMain::Start(const std::string& i_configuration_file)
  {
  mh_state_machine.reset(new TServerFSM(this));

  TiXmlDocument document;
  if (!XmlUtilities::LoadXmlDocument(i_configuration_file, document))
    {
    std::cout << "Loading of configuration file failed." << std::endl;
    return false;
    }

  const TiXmlElement& root = *document.RootElement();

  // Plug ins
  if (true)
    {
    const TiXmlElement* p_plugins = root.FirstChildElement("Plugins");
    
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::string plugins_directory = XmlUtilities::GetStringAttribute(p_plugins, "Directory", "server/plugins");
    mh_dll_manager.reset(new DllManager(converter.from_bytes(plugins_directory )));
    
    const TiXmlElement* p_child = nullptr;
    while (p_child = XmlUtilities::IterateChildElements(p_plugins, p_child))
      {
      std::string plugin_name = p_child->FirstChild()->Value();
      mh_dll_manager->LoadLib(converter.from_bytes(plugin_name));
      }
    }

  // network
  // start connection
  if (true)
    {
    const TiXmlNode* p_network = root.FirstChild("Network");
    int port = _GetPort(p_network->FirstChild("Port")->FirstChild()->Value());

    if ( !net::InitializeSockets() )
      {
      std::cout << "Failed to initialize sockets." << std::endl;
      return false;
      }

    mh_server_connection.reset(new net::Connection(net::ProtocolId, net::TimeOut));
    if(!mh_server_connection->Start(port))
      {
      std::cout << "Failed to start connection." << std::endl;
      return false;
      }
    
    mh_server_connection->Listen();
    }

  // Game
  if (true)
    {
    const TiXmlElement* p_game = root.FirstChildElement("Game");

    auto get_path = [](const TiXmlElement* p_path_element, const std::string& i_path_name) -> std::string
      {
      std::string path_type = XmlUtilities::GetStringAttribute(p_path_element->FirstChildElement(i_path_name.c_str()), "Type", "Relative");
      std::string path      = XmlUtilities::GetStringAttribute(p_path_element->FirstChildElement(i_path_name.c_str()), "File", "");

      using namespace Slavs;
      std::string game_path = "";
      if (path_type == "Relative")
        game_path = FileUtilities::JoinPath(FileUtilities::GetApplicationDirectory(), path);
      else
        game_path = path;
      return game_path;
      };

    Slavs::LoadingParameters loading_parameters;
    loading_parameters.m_map_path = get_path(p_game, "GamePath");
    loading_parameters.m_configurations_path = get_path(p_game, "ConfigurationsPath");
    
    mh_state_machine->SetCurrentState(std::make_shared<Slavs::LoadGameState>(loading_parameters));
    }

  m_bWorking = true;
  return true;
  }

void ServerMain::Stop()
  {
  m_bWorking = false;
  }