#ifndef ServerLoadGameState_h
#define ServerLoadGameState_h

#include "IController.h"

#include <Patterns/State.h>
#include <map>
#include <memory>

class ServerMain;
class SGameContext;

class ServerLoadGameState : public State<ServerMain, long>
{
public:
	ServerLoadGameState(std::shared_ptr<std::map<int, IController*>> controllers);
	~ServerLoadGameState();

  void          Enter(ServerMain* ip_owner);
  void          Execute(ServerMain* ip_owner, long i_elapsed_time);
  void          Exit(ServerMain* ip_owner);
  
  void          HoldPacket(ServerMain* ip_owner, unsigned char* ip_packet, size_t i_bytes_read);
protected:
	bool          m_bExitState;
	SGameContext* m_pGameContext;

	std::shared_ptr<std::map<int, IController*>> m_pControllers;
};

#endif