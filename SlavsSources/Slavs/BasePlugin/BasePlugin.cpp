// BasePlugin.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include "SlavsBasePluginAPI.h"
#include "SlavsBasePlugin.h"

#include <Main/ServerMain.h>

#include <Patterns/Singleton.h>

//////////////////////////////////////////////////////////////////////////

SlavsBasePlugin* g_plugin = nullptr;

//////////////////////////////////////////////////////////////////////////

extern "C" void SLAVS_BASEPLUGIN_EXPORT dllStartup(void)
  {
  // Create new scene manager
  g_plugin = new SlavsBasePlugin();
  printf("\n%s started\n", g_plugin->GetName().c_str());
  // Register
  ServerMain::GetInstance().RegisterPlugin(g_plugin);
  }
extern "C" void SLAVS_BASEPLUGIN_EXPORT dllShutdown(void)
  {
  printf("\n%s released\n", g_plugin->GetName().c_str());
  ServerMain::GetInstance().UnregisterPlugin(g_plugin);
  delete g_plugin;
  }