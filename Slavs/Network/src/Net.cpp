#include "Net.h"

#if PLATFORM == PLATFORM_WINDOWS

#include <winsock2.h>
#pragma comment( lib, "wsock32.lib" )

#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX

#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#else

#error unknown platform!

#endif

namespace net
{

  inline bool InitializeSockets()
  {
#if PLATFORM == PLATFORM_WINDOWS
    WSADATA WsaData;
    return WSAStartup( MAKEWORD(2,2), &WsaData ) == NO_ERROR;
#else
    return true;
#endif
  }

  inline void ShutdownSockets()
  {
#if PLATFORM == PLATFORM_WINDOWS
    WSACleanup();
#endif
  }

} // net