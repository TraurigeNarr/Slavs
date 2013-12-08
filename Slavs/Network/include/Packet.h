#ifndef Packet_h
#define Packet_h

#include <Game/Enumerations.h>

namespace net
{
	class Packet
	{
		PacketType m_PacketType;
		char *m_pData;
	public:
		Packet(PacketType ptype, char* data);
		char* GetData();
		PacketType GetType();
	};
}
#endif