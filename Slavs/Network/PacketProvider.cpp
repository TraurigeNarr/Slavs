#include "stdafx.h"

#include "PacketProvider.h"
#include "Packet.h"

#include "include/Net.h"

#include <Common/Utilities/TemplateFunctions.h>

namespace Network
  {

  PacketProvicer::PacketProvicer(net::Connection& i_connection)
    : mp_buffer(new unsigned char[PACKET_SIZE])
    , m_connection(i_connection)
    , m_buffer_size(PACKET_SIZE)
    {    }

  PacketProvicer::~PacketProvicer()
    {
    mp_buffer.reset();
    }

  Packet PacketProvicer::GetNextPacket() const
    {
    int bytes_read = m_connection.ReceivePacket( &mp_buffer[0], m_buffer_size);

    if (bytes_read == 0)
      return Packet();

    PacketType packet_type = ConvertFrom<PacketType> ( mp_buffer.get() );
    bytes_read -= sizeof(PacketType);

    assert (bytes_read >= 0);

    size_t packet_data_size = bytes_read;
    void* p_data = bytes_read != 0 ? &mp_buffer[sizeof(PacketType)] : nullptr;

    return Packet(packet_type, packet_data_size, p_data);
    }

  } // Network