#include "stdafx.h"

#include "SlavsBasePlugin.h"

#include <string>

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
  printf ("\ninstalled\n");
  }

/// performs logical initialization based on allocated resources
void SlavsBasePlugin::Initialize()
  {
  printf ("\ninitialized\n");
  }

/// performs logical releasing 
void SlavsBasePlugin::Release()
  {
  printf ("\nreleased\n");
  }

/// performs memory and resources deletion
void SlavsBasePlugin::Uninstall()
  {
  printf ("\nuninstalled\n");
  }