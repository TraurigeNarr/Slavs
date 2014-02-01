#ifndef ServerGameState_h
#define ServerGameState_h

#include "IController.h"

#include <Patterns/State.h>
#include <map>
#include <memory>

class SGameContext;
class ServerMain;

//need tick time in ServerGameState.cpp
class ServerGameState : public State<ServerMain, long>
{
public:
  typedef std::shared_ptr<TimeController> TTimeController;
public:
	ServerGameState(SGameContext*, std::shared_ptr<std::map<int, IController*>> controllers);
	~ServerGameState();

	void          Enter(ServerMain* ip_owner);
	void          Execute(ServerMain* ip_owner, long i_elapsed_time);
	void          Exit(ServerMain* ip_owner);

	void          HoldPacket(ServerMain* ip_owner, unsigned char *packet, size_t bytes_read);
  SGameContext& GetContext() { return *m_pSContext; }

  TTimeController GetTimeController() const;
protected:
	void          SendStates(ServerMain* ip_owner);
	void          PassToController(ServerMain* ip_owner, unsigned char *packet, size_t bytes_read);
	
  
	std::shared_ptr<std::map<int, IController*>> m_pControllers;

  bool          m_bExitState;
	SGameContext* m_pSContext;
  size_t        m_last_sent;

  TTimeController mp_time_controller;
};

#endif