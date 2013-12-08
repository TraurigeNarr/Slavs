#include "Connection.h"

#include <Utilities/TemplateFunctions.h>

#include <cassert>
#include <stdio.h>

namespace net
{

  Connection::Connection( unsigned int i_protocolId, float i_timeout )
    : m_protocolId(i_protocolId)
    , m_timeout(i_timeout)
    , m_mode(CM_None)
    , m_running(false)
    , m_timeout_accumulator(0)
  {
    _ClearData();
  }

  Connection::~Connection()
  {
    if (m_running)
      Stop();
  }

  bool Connection::Start( int i_port )
  {
    assert( !m_running );
    printf( "start connection on port %d\n", i_port );
    if ( !m_socket.Open(i_port) )
      return false;
    m_running = true;
    return true;
  }

  void Connection::Stop()
  {
    assert( m_running );
    printf( "stop connection\n" );
    _ClearData();
    m_socket.Close();
    m_running = false;
  }

  void Connection::Listen()
  {
    printf( "server listening for connection\n" );
    _ClearData();
    m_mode = CM_Server;
    m_state = CS_Listening;
  }

  void Connection::Connect(const Address& i_address)
  {
    printf( "client connecting to %d.%d.%d.%d:%d\n", 
      i_address.GetA(), i_address.GetB(), i_address.GetC(), i_address.GetD(), i_address.GetPort() );
    _ClearData();
    m_mode = CM_Client;
    m_state = CS_Connecting;
    m_address = i_address;
  }

  bool Connection::IsConnecting() const
  {
    return m_state == CS_Connecting;
  }

  bool Connection::ConnectFailed() const
  {
    return m_state == CS_ConnectFail;
  }

  bool Connection::IsConnected() const
  {
    return m_state == CS_Connected;
  }

  bool Connection::IsListening() const
  {
    return m_state == CS_Listening;
  }

  Connection::ConnectionMode Connection::GetMode() const
  {
    return m_mode;
  }

  void Connection::Update( float i_deltaTime )
  {
    assert( m_running );
    m_timeout_accumulator += i_deltaTime;
    if ( m_timeout_accumulator > m_timeout )
    {
      if ( m_state == CS_Connecting || m_state == CS_Connected)
      {
        printf( "connect timed out\n" );
        _ClearData();
        m_state = CS_ConnectFail;
      }
    }
  }

  bool Connection::SendPacket( const void* ip_data/*const unsigned char data[]*/, size_t i_size )
  {
    assert( m_running );
    if ( m_address.GetAddress() == 0 )
      return false;
    unsigned char *packet = new unsigned char[i_size+2*sizeof(int)];
    packet[0] = (unsigned char) (   m_protocolId >> 24 );
    packet[1] = (unsigned char) ( ( m_protocolId  >> 16 ) & 0xFF );
    packet[2] = (unsigned char) ( ( m_protocolId  >> 8 ) & 0xFF );
    packet[3] = (unsigned char) ( ( m_protocolId  ) & 0xFF );
    //write packet id
    ToChar(++m_iPacketSentID, (char*)&packet[4], sizeof(int));
    memcpy( &packet[8], ip_data, i_size );
    bool f = m_socket.Send( m_address, packet, i_size + 2*sizeof(int) );
    delete []packet;
    return f;
  }

  int Connection::ReceivePacket( void* ip_data, size_t i_size )
  {
    assert( m_running );
    unsigned char *packet = new unsigned char[i_size+2*sizeof(int)];
    Address sender;
    int bytes_read = m_socket.Receive( sender, packet, i_size + 4 );
    if ( bytes_read == 0 )
    {
      delete[] packet;
      return 0;
    }
    if ( bytes_read <= 8 )
    {
      delete[] packet;
      return 0;
    }
    if ( packet[0] != (unsigned char) (   m_protocolId >> 24 ) || 
      packet[1] != (unsigned char) ( ( m_protocolId >> 16 ) & 0xFF ) ||
      packet[2] != (unsigned char) ( ( m_protocolId >> 8 ) & 0xFF ) ||
      packet[3] != (unsigned char) (   m_protocolId & 0xFF ) )
    {
      delete[] packet;
      return 0;
    }
    int prevPacket = m_iPacketRecievedID;
    m_iPacketRecievedID = FromChar<int>((char*)(packet + 4));

    if(m_iPacketRecievedID != prevPacket + 1)
      printf("Packet id is not correct\n");

    if ( m_mode == CM_Server && !IsConnected() )
    {
      printf( "server accepts connection from client %d.%d.%d.%d:%d\n", 
        sender.GetA(), sender.GetB(), sender.GetC(), sender.GetD(), sender.GetPort() );
      m_state = CS_Connected;
      m_address = sender;
    }
    if ( sender == m_address )
    {
      if ( m_mode == CM_Client && m_state == CS_Connecting )
      {
        printf( "client completes connection with server\n" );
        m_state = CS_Connected;
      }
      m_timeout_accumulator = 0.0f;
      memcpy( ip_data, &packet[8], i_size - 8 );
      delete []packet;
      return bytes_read - 8;
    }
    delete []packet;
    return 0;
  }

  Address Connection::GetAddress() const 
  { 
    return m_address; 
  }

  void	Connection::Disconnect()
  {
    _ClearData();
  }

  void Connection::_ClearData()
  {
    m_state = CS_Disconnected;
    m_timeout_accumulator = 0.0f;
    m_address = Address();
    m_iPacketSentID = m_iPacketRecievedID = -1;
  }

} // net