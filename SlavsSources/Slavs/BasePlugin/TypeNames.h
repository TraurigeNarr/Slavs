#pragma once

namespace BasePlugin
  {

  // objects
  const std::string Object_Human                = "HumanObject";
  const std::string Object_Hiszhina             = "HizhinaObject";
  const std::string Object_ProductionStore      = "ProductionStoreObject";
  const std::string Object_Manufacture          = "ManufactureObject";
	const std::string Object_Tree									= "TreeObject";
	const std::string Object_Lesovik							= "LesovikObject";

  // components
  const std::string Component_Static            = "StaticComponent";
  const std::string Component_Dynamic           = "DynamicComponent";

  const std::string Component_Human             = "HumanComponent";
  const std::string Component_Store             = "StoreComponent";
  const std::string Component_House             = "HouseComponent";
  const std::string Component_Manufacture       = "ManufactureComponent";
  const std::string Component_PrimitiveBrain    = "PrimitiveBrain";
	const std::string Component_Lesovik						= "LesovikComponent";

	const std::string Component_Resource					= "ResourceComponent";

  //return BasePlugin. + i_name
  inline std::string ConstructBasePluginName(const std::string& i_name)
    {
    return "BasePlugin." + i_name;
    }

  } // BasePlugin