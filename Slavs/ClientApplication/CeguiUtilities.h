#pragma once

#include <CEGUI/InputEvent.h>
#include <OIS/OISMouse.h>

namespace CeguiUtilities
  {

  CEGUI::MouseButton ConvertButton(OIS::MouseButtonID buttonID);

  } // CeguiUtilities