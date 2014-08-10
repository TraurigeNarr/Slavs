#pragma once

namespace BasePlugin
  {

  template <typename MovingObject>
  bool MovingGroup<MovingObject>::AddMovingObject (MovingObject* ip_moving_object)
    {
    return m_moving_objects.insert(ip_moving_object).second;
    }

  template <typename MovingObject>
  void  MovingGroup<MovingObject>::RemoveMovingObject (MovingObject* ip_moving_object)
    {
    m_moving_objects.erase(ip_moving_object);
    }

  template <typename MovingObject>
  bool MovingGroup<MovingObject>::Empty() const
    {
    return m_moving_objects.empty();
    }

  template <typename MovingObject>
  void MovingGroup<MovingObject>::SetMovingType(MovingType i_type)
    {
    m_moving_type = i_type;
    }

  template <typename MovingObject>
  MovingType MovingGroup<MovingObject>::GetMovingType() const
    {
    return m_moving_type;
    }

  } // BasePlugin