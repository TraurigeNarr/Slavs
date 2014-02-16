#include "stdafx.h"

#include "SlavsBasePlugin.h"
#include "BaseObjectComposer.h"

#include <ServerMain.h>
#include <PluginSystem/MetaFactory.h>

#include <string>
#include <iostream>

//////////////////////////////////////////////////////////////////////////

const std::string PLUGIN_NAME = "SlavsBasePlugin";

//////////////////////////////////////////////////////////////////////////

SlavsBasePlugin::SlavsBasePlugin()
  {

  }

SlavsBasePlugin::~SlavsBasePlugin()
  {

  }

std::string SlavsBasePlugin::GetName() const
  {
  return PLUGIN_NAME;
  }

/// performs memory and resources allocations
void SlavsBasePlugin::Install()
  {
  std::cout << "Plugin: " << PLUGIN_NAME << " installed." << std::endl;
  mh_object_composer.reset(new BaseObjectComposer);
  }

/// performs logical initialization based on allocated resources
void SlavsBasePlugin::Initialize()
  {
  std::cout << "Plugin: " << PLUGIN_NAME << " initialized." << std::endl;
  ServerMain::GetInstance().GetMetaFactory().RegisterObjectComposer(mh_object_composer);  
  }

/// performs logical releasing 
void SlavsBasePlugin::Release()
  {
  std::cout << "Plugin: " << PLUGIN_NAME << " released." << std::endl;
  ServerMain::GetInstance().GetMetaFactory().UnregisterComposer(mh_object_composer);  
  }

/// performs memory and resources deletion
void SlavsBasePlugin::Uninstall()
  {
  std::cout << "Plugin: " << PLUGIN_NAME << " uninstalled." << std::endl;
  mh_object_composer = nullptr;
  }