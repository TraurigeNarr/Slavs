#pragma once

class MessageDispatcher;

class Application;

namespace ClientStates
  {
  class GameState;
  } // ClientStates

namespace UI
  {

  struct ButtonPressed;
  struct CommandButtonPressed;

  class GameBaseCommandHandler
    {
    private:
      MessageDispatcher&                m_dispatcher;
      Application&                      m_application;
      ClientStates::GameState&          m_game_state;
    public:
      GameBaseCommandHandler(ClientStates::GameState& i_state, Application& i_application, MessageDispatcher& i_dispatcher);
      ~GameBaseCommandHandler();

      void ButtonPressedHandler (const ButtonPressed& i_button_pressed);
      void CommandButtonPressedHandler(const CommandButtonPressed& i_command_button_pressed);
    };

  } // UI