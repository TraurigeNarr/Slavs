#pragma once

#include <string>

namespace Slavs
  {

  struct LoadingParameters
    {
    std::string m_map_path;
    std::string m_configurations_path;
    // may be some other parameters:
    //  1. Connections
    //  2. Players
    //  3. Settings
    };

  } // Slavs