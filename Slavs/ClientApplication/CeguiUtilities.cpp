#include "stdafx.h"

#include "CeguiUtilities.h"

namespace CeguiUtilities
  {

  CEGUI::MouseButton ConvertButton(OIS::MouseButtonID i_button_id)
    {
    switch (i_button_id)
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

  void ClearWindow(CEGUI::Window& i_window, bool i_destroy)
    {
    std::vector<CEGUI::Window*> children;

    CEGUI::WindowManager* p_window_manager = CEGUI::WindowManager::getSingletonPtr();
    for(size_t i = 0; i < i_window.getChildCount(); ++i)
      children.push_back(i_window.getChildAtIdx(i));

    for(size_t i = 0; i < children.size(); ++i)
      p_window_manager->destroyWindow(children.at(i));

    if (i_destroy)
      p_window_manager->destroyWindow(&i_window);
    }

  } // CeguiUtilities