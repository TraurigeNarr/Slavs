#ifndef _SOCKET_H_
#define _SOCKET_H_

#include "NetworkAPI.h"

#include "Address.h"

namespace net
{

  class NETWORKEXPORT Socket
  {
  public:
    Socket();
    ~Socket();

    bool    Open( unsigned short i_port );
    void    Close();
    bool    IsOpen() const;

    bool    Send( const Address& i_destination, const void* ip_data, int i_size );
    int     Receive( Address& o_sender, void* ip_data, size_t i_size );

  private:
    int     m_socket;
  };

} // net

#endif