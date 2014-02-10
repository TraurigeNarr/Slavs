#include "ServerMain.h"

#include <Patterns/Singleton.h>

#include <Windows.h>
#include <ctime>
#include <exception>
#include <iostream>

template<> std::shared_ptr<ServerMain> Singleton<ServerMain>::mp_singleton = nullptr;

int main()
  {
 	
  new Singleton<ServerMain>(new ServerMain());
 
  ServerMain& server = Singleton<ServerMain>::GetInstance();

  try
    {
    if(!server.Initialize())
      {
      throw std::runtime_error("Failed to initialize server.");
      }
    long start = 0;
    long elapsedTime = 0;
    DWORD sleepTime = 0;
    while(server.Working())
      {
      start = clock();
      server.Update(10);
      elapsedTime = clock() - start;
      //10 ms sleep
      Sleep( 10 ); 
      }

    server.Shutdown();
    }
  catch (std::exception& e)
    {
    std::cerr << e.what() << std::endl;
    Singleton<ServerMain>::ReleaseIfValid();
    return 1;
  	}

  Singleton<ServerMain>::ReleaseIfValid();
	return 0;
  }