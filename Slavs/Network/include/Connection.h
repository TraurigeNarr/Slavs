#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include "NetworkAPI.h"

#include "Address.h"
#include "Socket.h"

namespace net
{

  class NETWORKEXPORT Connection
  {
  public:

    enum ConnectionMode
    {
      CM_None,	
      CM_Client,
      CM_Server
    };

    Connection( unsigned int i_protocolId, float i_timeout );

    ~Connection();

    bool            Start( int i_port );
    void            Stop();
    void            Listen();

    void            Update( float i_deltaTime );
    void            Connect(const Address& i_address );
    void            Disconnect();

    bool            IsConnecting() const;
    bool            IsConnected() const;
    bool            IsListening() const;

    bool            ConnectFailed() const;
    ConnectionMode  GetMode() const;
    Address         GetAddress() const;

    bool            SendPacket( const void* ip_data/*const unsigned char data[]*/, size_t i_size );
    int             ReceivePacket( void* ip_data, size_t i_size );


  protected:

    void            _ClearData();

  private:

    enum ConnectionState
    {
      CS_Disconnected,
      CS_Listening,
      CS_Connecting,
      CS_ConnectFail,
      CS_Connected
    };

    unsigned int      m_protocolId;
    float             m_timeout;
    bool              m_running;
    float             m_timeout_accumulator;
    int               m_iPacketRecievedID;
    int               m_iPacketSentID;
    ConnectionMode    m_mode;
    ConnectionState   m_state;
    Socket            m_socket;
    Address           m_address;
  };

} // net

#endif