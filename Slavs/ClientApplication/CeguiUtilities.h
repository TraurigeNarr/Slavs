#pragma once

#include <CEGUI/InputEvent.h>
#include <OIS/OISMouse.h>

namespace CeguiUtilities
  {

  CEGUI::MouseButton ConvertButton(OIS::MouseButtonID i_button_id);
  void ClearWindow(CEGUI::Window& i_window, bool i_destroy = false);

  } // CeguiUtilities