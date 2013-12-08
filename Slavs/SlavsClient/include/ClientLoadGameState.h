#ifndef ClientLoadGameState_h
#define ClientLoadGameState_h

#include "AppState.h"

class Application;
class CGameContext;

class ClientLoadGameState : public AppState
{
public:
  //mask for selecting own units
	ClientLoadGameState(int i_own_mask);
	~ClientLoadGameState();

	virtual void Enter(std::shared_ptr<Application> ip_owner) override;
	virtual void Execute(std::shared_ptr<Application> ip_owner, long i_frame_event) override;
	virtual void Exit(std::shared_ptr<Application> ip_owner) override;

protected:
	enum { CLState_ClientReady = 0x00, CLState_Loading = 0x01, CLStateGoToGame = 0x02};
	void _HoldPacket(std::shared_ptr<Application> ip_owner, unsigned char *packet);
	void _SendReadyPacket(std::shared_ptr<Application> ip_owner);

	CGameContext *m_pContext;
	char m_State;
};

#endif