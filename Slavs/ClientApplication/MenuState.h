#pragma once

#include "ApplicationStateBase.h"

namespace UI
  {
  class MenuCommandHandler;
  }

namespace ClientStates
  {

  class MenuState : public ApplicationStateBase
    {
    private:
      std::unique_ptr<UI::MenuCommandHandler> mp_command_handler;

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
    };

  } // ClientStates