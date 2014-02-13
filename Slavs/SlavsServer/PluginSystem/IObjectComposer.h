#pragma once

#include "SlavsServerAPI.h"

class SGameObject;

class SLAVS_SERVER_EXPORT IObjectComposer
  {
  public:
    virtual ~IObjectComposer(){}

    /// add components to game object
    /// @param ip_object - created object (memory must be allocated)
    virtual void ComposeObject (SGameObject* ip_object) = 0;
    virtual bool Supports (int i_object_type) = 0;

  };