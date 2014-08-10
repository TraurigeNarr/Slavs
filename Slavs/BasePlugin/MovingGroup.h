#pragma once

#include "SlavsBasePluginAPI.h"

#include <set>

namespace BasePlugin
  {
  class DynamicObjectComponent;
  enum class MovingType
    {
    MT_Unordered = 0
    };

  template <class MovingObject>
  class MovingGroup
    {
    private:
      std::set<MovingObject*> m_moving_objects;
      MovingType              m_moving_type;

    public:
      bool        AddMovingObject (MovingObject* ip_moving_object);
      void        RemoveMovingObject (MovingObject* ip_moving_object);

      bool        Empty() const;

      void        SetMovingType(MovingType i_type);
      MovingType  GetMovingType() const;
    };


  } // BasePlugin

#include "MovingGroupImpl.h"