//server
#include "ServerMain.h"
#include "PluginSystem\DllManager.h"
#include "PluginSystem\Plugin.h"
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

std::shared_ptr<ServerMain> ServerMain::mh_instance= nullptr;

ServerMain::ServerMain()
  : mh_server_connection(nullptr)
  , m_bWorking(false)
  , mh_dll_manager(nullptr)
  {
  assert (mh_instance.get() == nullptr);
  mh_instance.reset(this);
  }

ServerMain::~ServerMain()
{
	net::ShutdownSockets();
  mh_instance = nullptr;
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
	Singleton<WaitState>::ReleaseIfValid();
  Singleton<GlobalServerState>::ReleaseIfValid();
  mh_dll_manager->ReleaseLibraries();
}

void ServerMain::Update(long elapsedTime)
{
	m_pFSM->Update(elapsedTime);
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
  catch (std::invalid_argument& except)
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

#include "GameContext.h"
#include "GameSerializer.h"
#include "GameObject.h"

#include <locale>
#include <codecvt>

bool ServerMain::Start(const std::string& i_configuration_file)
  {
  TiXmlDocument document;
  if (!XmlUtilities::LoadXmlDocument(i_configuration_file, document))
    return false;

  const TiXmlElement& root = *document.RootElement();

  // network
  const TiXmlNode* p_network = root.FirstChild("Network");
  int port = _GetPort(p_network->FirstChild("Port")->FirstChild()->Value());

  // Plugins
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

  // Players
  if (true)
    {
    const TiXmlElement* p_players = root.FirstChildElement("Players");
    }

  // Game
  if (true)
    {
    const TiXmlElement* p_game = root.FirstChildElement("Game");
    std::string path_type = XmlUtilities::GetStringAttribute(p_game->FirstChildElement("Path"), "Type", "Relative");
    std::string path      = XmlUtilities::GetStringAttribute(p_game->FirstChildElement("Path"), "File", "");

    using namespace Slavs;
    GameContext context;
    GameSerializer serializer(context);
    std::string game_path = "";
    if (path_type == "Relative")
      game_path = FileUtilities::JoinPath(FileUtilities::GetApplicationDirectory(), path);
    else
      game_path = path;
    serializer.Load(game_path);
    }

  return true;
  }