#pragma once

namespace net
  {
  class Connection;
  }

namespace Network
  {

  struct Packet;

  class PacketProvicer
    {
    private:
      std::unique_ptr<unsigned char[]>  mp_buffer;
      size_t                            m_buffer_size;

      net::Connection&                  m_connection;

    public:
      PacketProvicer (net::Connection& i_connection);
      ~PacketProvicer();

      Packet           GetNextPacket() const;
      net::Connection& GetConnection();
    };

  //////////////////////////////////////////////////////////////////////////

  inline net::Connection& PacketProvicer::GetConnection()
    {
    return m_connection;
    }

  } // Network