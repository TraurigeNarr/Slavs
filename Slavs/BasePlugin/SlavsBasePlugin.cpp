#include "stdafx.h"

#include "SlavsBasePlugin.h"
#include "BaseObjectComposer.h"

#include <ServerMain.h>
#include <PluginSystem/MetaFactory.h>

#include <Utilities/FileUtilities.h>
#include <Utilities/XmlUtilities.h>

#include <string>
#include <iostream>

//////////////////////////////////////////////////////////////////////////

const std::string PLUGIN_NAME               = "SlavsBasePlugin";
const std::string PLUGIN_CONFIGURATION_FILE = "server\\configs\\SlavsBasePlugin.xml";

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


  TiXmlDocument document;

  if(!XmlUtilities::LoadXmlDocument(FileUtilities::JoinPath(FileUtilities::GetApplicationDirectory(), PLUGIN_CONFIGURATION_FILE), document))
    throw std::invalid_argument("Can not load file");
  
  const TiXmlElement* p_root = document.RootElement();

  std::string element_name = "";
  const TiXmlElement* p_child = 0;

  while ((p_child = XmlUtilities::IterateChildElements(p_root, p_child)))
    {
    element_name = p_child->Value();
    if (element_name == "Composer")
      {
      mh_object_composer->Initialize(*p_child);
      }
    }
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