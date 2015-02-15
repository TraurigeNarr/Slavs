#include "stdafx.h"

#include "GameBaseCommandHandler.h"

#include "Application.h"
#include "UIEvents.h"
#include "GameState.h"
#include "MenuState.h"

#include "Task.h"

#include "Application.h"
#include "MouseManager.h"
#include "MouseTypes.h"

#include <Patterns/MessageDispatcher/MessageDispatcher.h>

#include <Common/Patterns/StateMachine.h>
#include <Common/Utilities/TemplateFunctions.h>

#include <Network/include/Connection.h>
#include <Network/PacketType.h>

using namespace ClientStates;

namespace Tasks
  {
  class PlaceBuilding : public SDK::Task
    {
    private:
      const int m_building_id;
      net::Connection& m_connection;

    public:
      PlaceBuilding(int i_task_id, int i_building_id, net::Connection& i_connection)
        : Task(i_task_id)
        , m_building_id(i_building_id)
        , m_connection(i_connection)
        {}

      virtual ~PlaceBuilding(){}

      virtual void CompleteImpl(const boost::any& i_value) override
        {
        net::Connection& connection = reinterpret_cast<ClientStates::GameState*>(ClientGame::appInstance.GetStateMachine().GetCurrentState().get())->GetConnection();
        Ogre::Vector3 position = boost::any_cast<Ogre::Vector3>(i_value);

        const size_t packet_size = sizeof(Network::PacketType) + sizeof(int) + sizeof(float) + sizeof(float);
        std::unique_ptr<char[]> p_buffer(new char[packet_size]);
        size_t offset = 0;
        ToChar(Network::PacketType::PT_Command, &p_buffer[0], sizeof(Network::PacketType));
        offset += sizeof(Network::PacketType);
        ToChar(m_building_id, &p_buffer[offset], sizeof(int));
        offset += sizeof(int);
        ToChar(position.x, &p_buffer[offset], sizeof(float));
        offset += sizeof(float);
        ToChar(position.z, &p_buffer[offset], sizeof(float));
        connection.SendPacket(&p_buffer[0], packet_size);
        }

      virtual void DiscardImpl(const boost::any& i_value) override
        {
        // we should do nothing here
        }
    };
  }

namespace UI
  {

  GameBaseCommandHandler::GameBaseCommandHandler (ClientStates::GameState& i_state, Application& i_application, MessageDispatcher& i_dispatcher)
    : m_game_state(i_state)
    , m_dispatcher(i_dispatcher)
    , m_application(i_application)
    {
    m_dispatcher.RegisterHandler<GameBaseCommandHandler, const ButtonPressed&>(*this, &GameBaseCommandHandler::ButtonPressedHandler);
    m_dispatcher.RegisterHandler<GameBaseCommandHandler, const CommandButtonPressed&>(*this, &GameBaseCommandHandler::CommandButtonPressedHandler);
    }

  GameBaseCommandHandler::~GameBaseCommandHandler()
    {
    m_dispatcher.UnregisterHandler<ButtonPressed>();
    }

  void GameBaseCommandHandler::ButtonPressedHandler(const ButtonPressed& i_button_pressed)
    {
    switch (i_button_pressed.m_button)
      {
      case ButtonID::BI_BACK_TO_MENU:
        m_application.GetStateMachine().ChangeState(std::make_shared<MenuState>(m_application));
        break;
      case ButtonID::BI_HELP:
        break;
      case ButtonID::BI_ECONOMY_PLUGIN_STATUS:
        break;
			case ButtonID::BI_INFORMATION_OK:
			case ButtonID::BI_INFORMATION_CANCEL:
				{
				char buffer[sizeof(Network::PacketType)];
				ToChar(Network::PacketType::PT_Achived, buffer, sizeof(Network::PacketType));
				net::Connection& connection = reinterpret_cast<ClientStates::GameState*>(ClientGame::appInstance.GetStateMachine().GetCurrentState().get())->GetConnection();
				connection.SendPacket(buffer, sizeof(buffer));
				}
				break;
      }
    }

  void GameBaseCommandHandler::CommandButtonPressedHandler(const CommandButtonPressed& i_command_button_pressed)
    {
    auto p_task = SDK::TaskPtr(new Tasks::PlaceBuilding(m_tasks.size(), i_command_button_pressed.m_command_id, m_game_state.GetConnection()));
    ClientGame::appInstance.GetMouseManager().SetActiveMouse(static_cast<int>(MouseType::PlaceBuilding), p_task.get());
    m_tasks.push_back(std::move(p_task));
    }

  } // UI