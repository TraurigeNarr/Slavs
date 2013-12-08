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

  void          Enter(std::shared_ptr<ServerMain> ip_owner);
  void          Execute(std::shared_ptr<ServerMain> ip_owner, long i_elapsed_time);
  void          Exit(std::shared_ptr<ServerMain> ip_owner);
protected:
	void          HoldPacket(std::shared_ptr<ServerMain> ip_owner, unsigned char *packet, size_t bytes_read);
	
  inline void   ClientConnects(std::shared_ptr<ServerMain> ip_owner);
	bool m_bExitState;

	std::shared_ptr<std::map<int, IController*>> m_pControllers;
};

#endif