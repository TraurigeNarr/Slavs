#include "stdafx.h"

#include "IMouse.h"

namespace GameCore
  {
  
  namespace UI
    {

    IMouse::IMouse(int i_id)
      : m_id(i_id)
      , m_is_active(false)
      {}

    IMouse::~IMouse()
      {}

    void IMouse::Activate(SDK::Task* ip_task)
      {
      if (!m_is_active)
        {
        m_is_active = true;
        ActivateImpl(ip_task);
        }
      }

    void IMouse::Deactivate()
      {
      if (m_is_active)
        {
        m_is_active = false;
        DeactivateImpl();
        }
      }

    } // UI

  } // GameCore