// ServiceStarter.cpp : Defines the entry point for the console application.
//
#include <Main/ServerMain.h>
#include <PluginSystem/DllManager.h>

#include <Windows.h>
#include <ctime>
#include <exception>
#include <iostream>
#include <memory>
#include <tchar.h>

//////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
  {
  new ServerMain();
  ServerMain& server = ServerMain::GetInstance();
  std::string configuration_xml(".\\server\\StartServer.xml");
  if (argc < 2)
    {
    std::cout << "No input file have been specialized." << std::endl
              << "ServiceStarter.exe <path_to_configuration_xml>" << std::endl
              << "Try to use default." << std::endl
              << "----------------------" << std::endl;
    }
  else
    configuration_xml = argv[1];

  try
    {
    // initialize
    if(!server.Start(configuration_xml))
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

