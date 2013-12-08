#ifndef ServerMain_h
#define ServerMain_h

#include <Net.h>
#include <Patterns/StateMachine.h>

#include <list>

//////////////////////////////////////////////////////////////////////////

class ServerMain
{
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
	int GetListOfMaps(const std::string& mapDir);
	net::Connection *m_pServerConnection;
	bool m_bWorking;

	StateMachine<ServerMain, long> *m_pFSM;
	std::list<char*> m_lMaps;
};

#endif