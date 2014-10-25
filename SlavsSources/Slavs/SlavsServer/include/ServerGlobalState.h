#ifndef GlobalServerState_h
#define GlobalServerState_h

#include <Patterns/State.h>

class ServerMain;
//checks client connection and update connection
class GlobalServerState : public State<ServerMain, long>
{
public:
	GlobalServerState();
	~GlobalServerState();

  void          Enter(ServerMain* ip_owner);
  void          Execute(ServerMain* ip_owner, long i_elapsed_time);
  void          Exit(ServerMain* ip_owner);
protected:
  void          HoldPacket(ServerMain* ip_owner, unsigned char *packet, size_t bytes_read);

	bool m_bExitState;
};

#endif