#include "stdafx.h"

#include "CeguiEventListener.h"

#include "CeguiUtilities.h"

namespace Cegui
  {

  bool CeguiEventListener::KeyPressed(const OIS::KeyEvent &keyEventRef) 
    { 
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    context.injectKeyDown((CEGUI::Key::Scan)keyEventRef.key);
    context.injectChar((CEGUI::Key::Scan)keyEventRef.text);
    return true;
    }

  bool CeguiEventListener::KeyReleased(const OIS::KeyEvent &keyEventRef) 
    { 
    CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp((CEGUI::Key::Scan)keyEventRef.key);
    CEGUI::System::getSingleton().getDefaultGUIContext().injectChar(keyEventRef.text);
    return true; 
    }

  bool CeguiEventListener::MouseMoved(const OIS::MouseEvent &evt) 
    { 
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    context.injectMouseMove(static_cast<float>(evt.state.X.rel), static_cast<float>(evt.state.Y.rel));
    // Scroll wheel
    if (evt.state.Z.rel)
      context.injectMouseWheelChange(evt.state.Z.rel / 120.0f);
    return false; 
    }

  bool CeguiEventListener::MousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id) 
    { 
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    context.injectMouseButtonDown(CeguiUtilities::ConvertButton(id));
    return false; 
    }

  bool CeguiEventListener::MouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id) 
    {
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    context.injectMouseButtonUp(CeguiUtilities::ConvertButton(id));
    return true;
    }

  } // UI