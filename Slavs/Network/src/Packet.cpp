#include "Packet.h"
#include "Net.h"

namespace net
{
	Packet::Packet(PacketType ptype, char* data)
		: m_PacketType(ptype), m_pData(data)
	{	}

	char* Packet::GetData()
	{
		return m_pData;
	}

	PacketType Packet::GetType()
	{
		return m_PacketType;
	}
}