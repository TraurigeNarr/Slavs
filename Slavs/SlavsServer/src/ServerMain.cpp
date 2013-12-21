//server
#include "ServerMain.h"
//states
#include "ServerGameState.h"
#include "ServerGlobalState.h"
#include "ServerLoadGameState.h"
#include "ServerWaitState.h"
//common
#include <Patterns/Singleton.h>
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

ServerMain::ServerMain()
{
	m_pServerConnection = NULL;
	m_bWorking = false;
}

ServerMain::~ServerMain()
{
	if(NULL != m_pServerConnection)
		delete m_pServerConnection;
	net::ShutdownSockets();
}

bool ServerMain::Initialize()
{
  FromGame = false;
  m_pFSM = new StateMachine<ServerMain, long>(std::shared_ptr<ServerMain>(this));
	//initialize sockets
	if ( !net::InitializeSockets() )
	{
		printf( "failed to initialize sockets\n" );
		return false;
	}

	m_pServerConnection = new net::Connection(net::ProtocolId, net::TimeOut);
	if(!m_pServerConnection->Start(net::ServerPort))
		return false;

  m_pServerConnection->Listen();
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
}

void ServerMain::Update(long elapsedTime)
{
	m_pFSM->Update(elapsedTime);
}

net::Connection* ServerMain::GetConnection () const
{
  return m_pServerConnection;
}

int ServerMain::GetListOfMaps(const std::string& mapDir)
{
	m_lMaps.push_back("test_01");
	return 0;
}