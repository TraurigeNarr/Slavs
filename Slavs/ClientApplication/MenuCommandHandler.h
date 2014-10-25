#pragma once

class MessageDispatcher;

class Application;

namespace UI
  {
  
  struct ButtonPressed;

  class MenuCommandHandler
    {
    private:
      MessageDispatcher&  m_dispatcher;
      Application&        m_application;

    public:
      MenuCommandHandler(Application& i_application, MessageDispatcher& i_dispatcher);
      ~MenuCommandHandler();

      void ButtonPressedHandler (const ButtonPressed& i_button_pressed);
    };

  } // UI