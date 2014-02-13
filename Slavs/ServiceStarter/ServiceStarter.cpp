// ServiceStarter.cpp : Defines the entry point for the console application.
//
#include <ServerMain.h>
#include <PluginSystem/DllManager.h>

#include <Windows.h>
#include <ctime>
#include <exception>
#include <iostream>
#include <memory>
#include <tchar.h>

//////////////////////////////////////////////////////////////////////////

int main(int argc, _TCHAR* argv[])
  {
  new ServerMain;
  ServerMain& server = ServerMain::GetInstance();
  server.GetDllManager().LoadLib(L"SlavsBasePlugin");
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
    return 1;
    }

  return 0;
  }

