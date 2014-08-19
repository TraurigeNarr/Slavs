#pragma once

#include "PacketType.h"

namespace Network
  {
  
  struct Packet
    {
    const PacketType  m_packet;
    const size_t      m_size;
    const void*       mp_data;

    Packet (PacketType i_packet, size_t i_size, const void* ip_data)
      : m_packet(i_packet)
      , m_size(i_size)
      , mp_data(ip_data)
      {      }

    Packet ()
      : m_packet(PacketType::PT_NO_PACKETS)
      , m_size(0)
      , mp_data(nullptr)
      {      }

    bool operator () ()
      {
      return PacketType::PT_NO_PACKETS != m_packet;
      }
    };

  } // Network