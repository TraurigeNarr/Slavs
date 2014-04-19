#ifndef ClientLoadGameState_h
#define ClientLoadGameState_h

#include "AppState.h"

#include <memory>

namespace net
  {
  class Address;
  }

class Application;
class CGameContext;

class ClientLoadGameState : public AppState
  {
  private:
    std::unique_ptr<net::Address> mp_server_address;
  private:
    std::unique_ptr<CGameContext> mh_game_context;

    enum 
      { 
      CLState_ClientReady = 0x00, 
      CLState_Loading     = 0x01, 
      CLStateGoToGame     = 0x02
      };
    char                          m_state;
    int                           m_current_content_number;
    int                           m_content_number;
  public:
    //mask for selecting own units
  	ClientLoadGameState(int i_own_mask);
  	~ClientLoadGameState();
  
    void         SetServerAddress(const net::Address& i_address);

  	virtual void Enter(Application* ip_owner) override;
  	virtual void Execute(Application* ip_owner, long i_frame_event) override;
  	virtual void Exit(Application* ip_owner) override;
  
  protected:
    bool Connect(Application* ip_owner, const net::Address& i_address);
  	void _HoldPacket(Application* ip_owner, unsigned char *packet, size_t i_bytes_read);
  	void _SendReadyPacket(Application* ip_owner);
  };

#endif