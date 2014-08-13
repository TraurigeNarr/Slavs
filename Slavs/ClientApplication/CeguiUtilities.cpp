#include "stdafx.h"

#include "CeguiUtilities.h"

namespace CeguiUtilities
  {

  CEGUI::MouseButton ConvertButton(OIS::MouseButtonID buttonID)
    {
    switch (buttonID)
      {
      case OIS::MB_Left:
        return CEGUI::LeftButton;
        break;

      case OIS::MB_Right:
        return CEGUI::RightButton;
        break;

      case OIS::MB_Middle:
        return CEGUI::MiddleButton;
        break;

      default:
        return CEGUI::LeftButton;
        break;
      }
    }

  } // CeguiUtilities