#ifndef ServerMain_h
#define ServerMain_h

#include "SlavsServerAPI.h"

#include <Net.h>
#include <Patterns/StateMachine.h>

#include <memory>

//////////////////////////////////////////////////////////////////////////

class DllManager;

class SLAVS_SERVER_EXPORT ServerMain
{
private:
  std::unique_ptr<DllManager> mh_dll_manager;

public:
	ServerMain();
	~ServerMain();

	bool Initialize();
	void Shutdown();

	void Update(long elapsedTime);

	bool Working(){ return m_bWorking; }

	net::Connection* GetConnection() const;
	StateMachine<ServerMain, long>* GetFSM() const { return m_pFSM; }
	bool FromGame;
private:
	net::Connection *m_pServerConnection;
	bool m_bWorking;

	StateMachine<ServerMain, long> *m_pFSM;
};

#endif