//this state
#include "ServerGlobalState.h"
//server
#include "ServerMain.h"

GlobalServerState::GlobalServerState(){}

GlobalServerState::~GlobalServerState(){}

void GlobalServerState::Enter(ServerMain* ip_owner)
{
	printf("Enters global state.\n");
	m_bExitState = false;
}

void GlobalServerState::Execute(ServerMain* ip_owner, long i_elapsed_time)
{
	if(m_bExitState)
		return;

	ip_owner->GetConnection()->Update(i_elapsed_time * 0.001f);
}

void GlobalServerState::Exit(ServerMain* ip_owner)
{
	printf("Exits global state.\n");
	m_bExitState = true;
}