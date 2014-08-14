#include "stdafx.h"

#include "Screen.h"
#include "ScreenManager.h"

namespace UI
  {

  Screen::Screen(ScreenManager& i_screen_manager, Screen* ip_owner /* = nullptr */)
    : m_screen_manager(i_screen_manager)
    {    }

  Screen::~Screen()
    {    }


  void Screen::Create(Screen* ip_owner /* = nullptr*/)
    {
    mp_owner = ip_owner;
    if (mp_owner)
      this->SetRootWindow(mp_owner->GetRootWindow());

    this->Initialize();

    }

  } // UI