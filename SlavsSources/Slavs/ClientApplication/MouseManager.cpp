#include "stdafx.h"

#include "MouseManager.h"
#include "IMouse.h"

#include "Task.h"

namespace GameCore
  {

  namespace UI
    {
    MouseManager::MouseManager()
      : mp_current_mouse(nullptr)
      , mp_previous_mouse(nullptr)
      {      }

    MouseManager::~MouseManager()
      {
      if (mp_current_mouse)
        mp_current_mouse->Deactivate();
      }

    void MouseManager::RegisterMouse(std::unique_ptr<IMouse> ip_mouse)
      {
      auto it = std::find_if(m_mice.begin(), m_mice.end(), IMouse::Comparer_Id(ip_mouse->GetId()));
      if (it != m_mice.end())
        throw std::logic_error("Same id already exist. Adding mice: " + ip_mouse->GetName());
      m_mice.push_back(std::move(ip_mouse));
      }

    void MouseManager::RegisterMouseAndActivate(std::unique_ptr<IMouse> ip_mouse, SDK::Task* ip_task)
      {
      const int id = ip_mouse->GetId();
      RegisterMouse(std::move(ip_mouse));
      SetActiveMouse(id, ip_task);
      }

    void MouseManager::SetActiveMouse(int i_id, SDK::Task* ip_task)
      {
      auto it = std::find_if(m_mice.begin(), m_mice.end(), IMouse::Comparer_Id(i_id));
      if (it == m_mice.end())
        throw std::logic_error("Cannot activate mouse. No mouse with id.");
      if (mp_current_mouse)
        {
        mp_current_mouse->Deactivate();
        mp_previous_mouse = mp_current_mouse;
        }

      mp_current_mouse = it->get();
      mp_current_mouse->Activate(ip_task);
      }

    void MouseManager::SetActiveMouse(const std::string& i_name, SDK::Task* ip_task)
      {
      auto it = std::find_if(m_mice.begin(), m_mice.end(), IMouse::Comparer_Name(i_name));
      if (it == m_mice.end())
        throw std::logic_error("Cannot activate mouse. No mouse with id.");
      if (mp_current_mouse)
        {
        mp_current_mouse->Deactivate();
        mp_previous_mouse = mp_current_mouse;
        }

      mp_current_mouse = m_mice.back().get();
      mp_current_mouse->Activate(ip_task);
      }

    void MouseManager::PopMouse()
      {
      if (mp_current_mouse)
        {
        mp_current_mouse->Deactivate();
        if (mp_previous_mouse)
          {
          mp_current_mouse = mp_previous_mouse;
					mp_current_mouse->Activate(nullptr);
          // TODO make Pause and Resume methods
          // mp_current_mouse->Activate();
          mp_previous_mouse = nullptr;
          }
        }
      }

    void MouseManager::PopAllMice()
      {
      if (mp_current_mouse)
        mp_current_mouse->Deactivate();

      mp_current_mouse = nullptr;
      mp_previous_mouse = nullptr;
      }

    void MouseManager::Update(long i_elapsed_time)
      {
      if (mp_current_mouse)
        mp_current_mouse->Update(i_elapsed_time);
      }
    }

  } // GameCore