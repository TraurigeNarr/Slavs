#pragma once

#include "SlavsServerAPI.h"

namespace Slavs
  {
  class GameObject;
  }

class SLAVS_SERVER_EXPORT IObjectComposer
  {
  public:
    virtual ~IObjectComposer(){}

    /// add components to game object
    /// @param ip_object - created object (memory must be allocated)
    virtual void ComposeObject (Slavs::GameObject* ip_object) = 0;
    virtual bool Supports (int i_object_type) = 0;

    /// call this before using composer
    /// register set of types and components in global factory
    virtual void DeclareSupportedTypes() = 0;

    /// checks if all dependencies were registered in system
    /// checks that all types are registered correctly
    virtual bool CheckContracts() const = 0;
  };