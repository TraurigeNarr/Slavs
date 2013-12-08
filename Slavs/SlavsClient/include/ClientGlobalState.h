#ifndef ClientGlobalState_h
#define ClientGlobalState_h

#include "AppState.h"

class Application;

class ClientGlobalState : public AppState
{
  bool m_exit_state;
private:
  void HoldPacket(std::shared_ptr<Application>, unsigned char *packet);
public:
	ClientGlobalState ();
	~ClientGlobalState ();

	virtual void Enter(std::shared_ptr<Application> ip_owner) override;
	virtual void Execute(std::shared_ptr<Application> ip_owner, long i_elapsed_time) override;
	virtual void Exit(std::shared_ptr<Application> ip_owner) override;
};

#endif