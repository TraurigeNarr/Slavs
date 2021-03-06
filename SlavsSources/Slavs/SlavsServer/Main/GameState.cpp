#include "GameState.h"

#include "GameStateFSM.h"

#include "Game\GameContext.h"
#include "Game\GameObject.h"
#include "LoadGameState.h"
#include "ServerMain.h"
#include "misc\ServerEnums.h"

#include "IGameConroller.h"
#include "IController.h"

#include "Management/Goverment.h"

#include <Utilities/TemplateFunctions.h>

#include <Network/Packet.h>
#include <Network/SerializableData.h>
#include <Network/include/Net.h>

#include <memory>

//////////////////////////////////////////////////////////////////////////

namespace
  {

  void SendPacket (const Network::Packet& i_packet, net::Connection& i_connection)
    {
    i_connection.SendPacket(i_packet.mp_data, i_packet.m_size+sizeof(Network::PacketType));
    }

  void SendControllerStates (const Slavs::GameContext& i_context, net::Connection& i_connection)
    {
    static std::unique_ptr<char[]> p_buffer(new char[PACKET_SIZE]);
    ToChar(Network::PacketType::PT_GovermentState, &p_buffer[0], sizeof(Network::PacketType));

    Network::SerializableData data(10, sizeof(float));
    Network::Packet packet(Network::PacketType::PT_GovermentState, PACKET_SIZE, &p_buffer[sizeof(Network::PacketType)]);

    auto& controllers = i_context.GetControllers();
    for (auto& controller : controllers)
      {
      auto& p_goverment = controller.mp_controller->GetGoverment();
      auto* p_economy_manager = p_goverment.GetEconomyManager();

      if (p_economy_manager->HasChanges())
        {        
        p_economy_manager->Serialize(data);
        p_economy_manager->ValidateChanges();
        data.Serialize(packet);
        i_connection.SendPacket(&p_buffer[0], data.GetSize()+sizeof(Network::PacketType));
        }
      }
    }

  void SendStates(const Slavs::GameContext& i_context, net::Connection& i_connection)
    {
    static std::unique_ptr<char[]> p_buffer(new char[PACKET_SIZE]);
    ObjectStateMap states_map;
    i_context.GetAllGameObjectsState(states_map);
    if (!states_map.empty())
      {
      std::for_each(states_map.begin(), states_map.end(), [&i_connection](std::pair<long, GameObjectState*> p)
        {
        size_t neededSize = p.second->NeededSize() + sizeof(Network::PacketType);
        ToChar(Network::PacketType::PT_GOState, &p_buffer[0], sizeof(Network::PacketType));

        p.second->Serialize(&p_buffer[sizeof(Network::PacketType)], neededSize);
        i_connection.SendPacket(&p_buffer[0], neededSize);
        });
      ClearSTLMap(states_map);
      }
    } 

	void SendEndGameMessage(net::Connection& i_connection, bool i_win)
		{
		static std::unique_ptr<char[]> p_buffer(new char[sizeof(Network::PacketType) + sizeof(bool)]);

		ToChar(Network::PacketType::PT_EndGame, &p_buffer[0], sizeof(Network::PacketType));
		ToChar(i_win, &p_buffer[sizeof(Network::PacketType)], sizeof(bool));
		i_connection.SendPacket(&p_buffer[0], sizeof(Network::PacketType) + sizeof(bool));
		}

  } // namespace

//////////////////////////////////////////////////////////////////////////

namespace Slavs
  {

	GameState::GameState(TGameContext ih_game_context)
		: mh_game_context(std::move(ih_game_context))
		, mp_time_controller(std::make_shared<TimeController>(MS_FOR_TICK))
		, m_game_controller(*this, std::bind(&GameState::SwitchState, this, std::placeholders::_1), static_cast<int>(State::Play))
		, m_game_state_FSM(this)
    {     }

  GameState::~GameState()
    {    }

	bool GameState::SwitchState(int i_state)
		{
		switch (i_state)
			{
			case State::Play:
				if (typeid(*m_game_state_FSM.GetCurrentState().get()) == typeid(PlayState))
					return true;
				m_game_state_FSM.ChangeState(std::make_shared<PlayState>());
				break;
			case State::PlayerDecision:
				if (typeid(*m_game_state_FSM.GetCurrentState().get()) == typeid(PlayerDecisionState))
					return true;
				m_game_state_FSM.ChangeState(std::make_shared<PlayerDecisionState>());
				break;
			default:
				return false;
			}

		return true;
		}

	GameState::State GameState::GetGameFSMState() const
		{
		if (typeid(*m_game_state_FSM.GetCurrentState().get()) == typeid(PlayState))
			return State::Play;
		if (typeid(*m_game_state_FSM.GetCurrentState().get()) == typeid(PlayerDecisionState))
			return State::PlayerDecision;
		}

  void GameState::Enter(ServerMain* ip_owner)
    {
    printf( "Enters GameState state\n" );
    
    ip_owner->GetStateMachine().SetPreviousState(nullptr);
    ip_owner->FromGame = true;

    mp_time_controller->AddSubscriber(mh_game_context.get());
		auto& controllers = ip_owner->GetMetaFactory().GetControllers();
		for (auto& p_controller : controllers)
			p_controller->GameBegin(&m_game_controller);

		m_game_state_FSM.SetCurrentState(std::make_shared<PlayState>());
    }

  void GameState::Execute(ServerMain* ip_owner, long i_elapsed_time)
    {
		m_game_controller.Update(i_elapsed_time);
		m_game_state_FSM.Update(i_elapsed_time);
    }

  void GameState::Exit(ServerMain* ip_owner)
    {
    printf( "Exits LoadGame state\n" );
		auto& controllers = ip_owner->GetMetaFactory().GetControllers();
		for (auto& p_controller : controllers)
			p_controller->GameEnd();
    mp_time_controller->RemoveSubscriber(mh_game_context.get());
    mh_game_context->ReleaseContext();
    ip_owner->GetConnection()->Disconnect();
    }

	/*void HandleCommand(SDK::GameCore::CommandManager& i_command_manager, IController* ip_controller, unsigned char* ip_packet, size_t i_bytes_read)
		{
		char* p_packet = reinterpret_cast<char*>(ip_packet);
		Network::PacketType packet_type = (Network::PacketType)FromChar<int>(p_packet);
		p_packet += sizeof(Network::PacketType);
		i_bytes_read -= sizeof(Network::PacketType);

		size_t offset = 0;
		int global_command_id = FromChar<int>(p_packet);
		offset += sizeof(int);

		if (auto p_executor = i_command_manager.GetExecutor(ip_controller, global_command_id))
			{
			float x_pos = FromChar<float>(p_packet + offset);
			offset += sizeof(float);
			float y_pos = FromChar<float>(p_packet + offset);
			i_command_manager.Execute(ip_controller, p_executor, global_command_id, Vector2D(x_pos, y_pos));
			}

		}
		*/
  void GameState::HoldPacket(ServerMain* ip_owner, unsigned char* ip_packet, size_t i_bytes_read)
    {
    
    }

  } // Slavs