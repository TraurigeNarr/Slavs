#pragma once

#include "InputSubscriber.h"

namespace Cegui
  {

  class CeguiEventListener : public InputSubscriber
    {
    public:
      virtual bool KeyPressed(const OIS::KeyEvent &keyEventRef) override;

      virtual bool KeyReleased(const OIS::KeyEvent &keyEventRef) override;

      virtual bool MouseMoved(const OIS::MouseEvent &evt) override;

      virtual bool MousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id) override;

      virtual bool MouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id) override;
    };

  } // UI