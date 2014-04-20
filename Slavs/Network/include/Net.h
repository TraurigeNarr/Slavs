/*
	Simple Network Library from "Networking for Game Programmers"
	http://www.gaffer.org/networking-for-game-programmers
	Author: Glenn Fiedler <gaffer@gaffer.org>
*/

#ifndef _NET_H_
#define _NET_H_

// platform detection

#define PLATFORM_WINDOWS  1
#define PLATFORM_MAC      2
#define PLATFORM_UNIX     3

#if defined(_WIN32)
#define PLATFORM PLATFORM_WINDOWS
#elif defined(__APPLE__)
#define PLATFORM PLATFORM_MAC
#else
#define PLATFORM PLATFORM_UNIX
#endif

#define PACKET_SIZE 256

#include "NetworkAPI.h"

#include "Address.h"
#include "Socket.h"
#include "Connection.h"

namespace net
{
	//some constants
	const int ServerPort = 30000;
	const int ClientPort = 30001;
	const int ProtocolId = 0x11112222;
	const float DeltaTime = 0.01f;
	const float TimeOut = 5.0f;

	bool NETWORKEXPORT InitializeSockets();
	void NETWORKEXPORT ShutdownSockets();
	
}

#endif
