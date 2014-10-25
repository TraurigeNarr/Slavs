#ifndef ClientGlobalState_h
#define ClientGlobalState_h

#include "AppState.h"

class Application;

class ClientGlobalState : public AppState
{
  bool m_exit_state;
private:
  void HoldPacket(Application*, unsigned char *packet);
public:
	ClientGlobalState ();
	~ClientGlobalState ();

	virtual void Enter(Application* ip_owner) override;
	virtual void Execute(Application* ip_owner, long i_elapsed_time) override;
	virtual void Exit(Application* ip_owner) override;
};

#endif