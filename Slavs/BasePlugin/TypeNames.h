#pragma once

namespace BasePlugin
  {

  // objects
  const std::string Object_Human      = "HumanObject";

  // components
  const std::string Component_Human   = "HumanComponent";
  const std::string Component_Static  = "StaticComponent";
  const std::string Component_Dynamic = "DynamicComponent";

  //return BasePlugin. + i_name
  inline std::string ConstructBasePluginName(const std::string& i_name)
    {
    return "BasePlugin." + i_name;
    }

  } // BasePlugin