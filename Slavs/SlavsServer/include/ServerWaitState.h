#ifndef WaitState_h
#define WaitState_h

#include "IController.h"

#include <Patterns/State.h>

#include <map>
#include <memory>

//////////////////////////////////////////////////////////////////////////

class ServerMain;

//////////////////////////////////////////////////////////////////////////

class WaitState : public State<ServerMain, long>
{
public:
	WaitState();
	~WaitState();

  void          Enter(ServerMain* ip_owner);
  void          Execute(ServerMain* ip_owner, long i_elapsed_time);
  void          Exit(ServerMain* ip_owner);
protected:
	void          HoldPacket(ServerMain* ip_owner, unsigned char *packet, size_t bytes_read);
	
  inline void   ClientConnects(ServerMain* ip_owner);
	bool m_bExitState;

	std::shared_ptr<std::map<int, IController*>> m_pControllers;
};

#endif