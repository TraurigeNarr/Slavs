#pragma once

#include "ApplicationStateBase.h"

namespace ClientStates
  {

  class MenuState : public ApplicationStateBase
    {
    public:
      MenuState(Application& i_application);
      virtual ~MenuState();
    
    // State
    public:
      virtual void Enter(Application* ip_owner) override;
      virtual void Execute(Application* ip_owner, long i_elapsed_time) override;
      virtual void Exit(Application* ip_owner) override;

    // InputSubscriber
    public:
      virtual bool KeyPressed(const OIS::KeyEvent &keyEventRef) override;
      virtual bool UiEvent (const UI::ControlData& i_ui_event) override;
    };

  } // ClientStates