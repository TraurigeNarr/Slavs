#pragma once

#include <Math/Vector2.h>

class IMovable
  {
  protected:
    Vector2D m_position;

  public:
    virtual   ~IMovable(){}

    void      SetPosition(const Vector2D& i_position)
      {
      m_position = i_position;
      }

    Vector2D  GetPosition() const
      {
      return m_position;
      }

    Vector2D& AccessPosition()
      {
      return m_position;
      }
  };