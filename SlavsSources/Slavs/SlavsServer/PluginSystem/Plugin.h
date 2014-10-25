#pragma once

#include "SlavsServerAPI.h"

#include <string>

class SLAVS_SERVER_EXPORT Plugin
  {
  public:
    Plugin(){}
    virtual ~Plugin(){}

    virtual std::string GetName() const = 0;

    /// performs memory and resources allocations
    virtual void Install() = 0;
    /// performs logical initialization based on allocated resources
    virtual void Initialize() = 0;
    /// performs logical releasing 
    virtual void Release() = 0;
    /// performs memory and resources deletion
    virtual void Uninstall() = 0;
  };