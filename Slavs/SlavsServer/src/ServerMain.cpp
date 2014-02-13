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
//net
#include <Net.h>
//standard
#include <ostream>
#include <cassert>

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
  , mh_dll_manager(new DllManager(L"server\\plugins"))
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
  ip_plugin->Initialize();
  }

void ServerMain::UnregisterPlugin(Plugin* ip_plugin)
  {
  m_plugins.erase(ip_plugin);
  ip_plugin->Release();
  ip_plugin->Uninstall();
  }

ServerMain& ServerMain::GetInstance()
  {
  assert (mh_instance.get());
  return *mh_instance.get();
  }