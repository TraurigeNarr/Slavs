#include "stdafx.h"

#include "ScreenManager.h"

#include "Screen.h"

namespace UI
  {

  ScreenManager::ScreenManager(InputManager& i_input_manager)
    : m_input_manager(i_input_manager)
    , mp_current_screen(nullptr)
    {    }

  ScreenManager::~ScreenManager()
    {    }

  void ScreenManager::_PushToStack(std::unique_ptr<Screen> ip_screen, Screen* ip_owner)
    {
    ip_screen->Create(ip_owner);
    m_screen_stack.push_back(std::move(ip_screen));
    mp_current_screen = m_screen_stack.back().get();
    }

  void ScreenManager::Update(long i_elapsed_time)
    {
    if (mp_current_screen)
      mp_current_screen->Update(i_elapsed_time);
    }

  void ScreenManager::AddScreenToStack(std::unique_ptr<Screen> ip_screen)
    {
    if (m_screen_stack.empty())
      SetCurrentScreen(std::move(ip_screen));

    Screen* p_owner_screen = m_screen_stack.back().get();
    _PushToStack(std::move(ip_screen), p_owner_screen);
    }

  void ScreenManager::SetCurrentScreen(std::unique_ptr<Screen> ip_screen)
    {
    RemoveScreenFromStack();
    _PushToStack(std::move(ip_screen), nullptr);
    }

  void ScreenManager::ClearAllScreens()
    {
    while (!m_screen_stack.empty())
      {
      RemoveScreenFromStack();
      }
    }

  void ScreenManager::RemoveScreenFromStack()
    {
    if (!m_screen_stack.empty())
      {
      ScreenPtr p_screen = std::move(m_screen_stack.back());
      p_screen->Destroy();
      m_screen_stack.pop_back();
      }
    }

  } // UI