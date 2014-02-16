#pragma once

namespace BasePlugin
  {

  // objects
  const std::string Object_Human    = "Human";

  // components
  const std::string Component_Human = "Human";


  //return BasePlugin. + i_name
  inline std::string ConstructBasePluginName(const std::string& i_name)
    {
    return "BasePlugin." + i_name;
    }

  } // BasePlugin