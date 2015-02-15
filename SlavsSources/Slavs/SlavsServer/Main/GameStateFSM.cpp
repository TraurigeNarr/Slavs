#include "GameStateFSM.h"
#include "GameState.h"

#include "Game\GameContext.h"
#include "Game\GameObject.h"
#include "LoadGameState.h"
#include "ServerMain.h"
#include "misc\ServerEnums.h"

#include "IGameConroller.h"
#include "IController.h"

#include "Management/Goverment.h"

#include <Common/Game/TimeController.h>

#include <Utilities/TemplateFunctions.h>

#include <Network/Packet.h>
#include <Network/SerializableData.h>
#include <Network/include/Net.h>

namespace
	{

	void SendPacket(const Network::Packet& i_packet, net::Connection& i_connection)
		{
		i_connection.SendPacket(i_packet.mp_data, i_packet.m_size + sizeof(Network::PacketType));
		}

	void SendControllerStates(const Slavs::GameContext& i_context, net::Connection& i_connection)
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
				i_connection.SendPacket(&p_buffer[0], data.GetSize() + sizeof(Network::PacketType));
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

namespace Slavs
	{

	///////////////////////////////////////////////////////////////////////////
	// PlayState
	PlayState::PlayState()
		{

		}

	void PlayState::Enter(GameState* ip_owner)
		{
		printf("Enter PlayState\n");
		}

	void PlayState::Execute(GameState* ip_owner, long i_elapsed_time)
		{
		ServerMain& serverMain = ServerMain::GetInstance();

		static std::unique_ptr<unsigned char[]> packet(new unsigned char[PACKET_SIZE]);
		while (true)
			{
			int bytes_read = serverMain.GetConnection()->ReceivePacket(&packet[0], PACKET_SIZE);
			if (0 == bytes_read)
				break;

			HoldPacket(ip_owner, &packet[0], bytes_read);
			}

		ip_owner->GetTimeController().Update(i_elapsed_time);
		SendStates(*ip_owner->GetContext(), *serverMain.GetConnection());

		// move to tick listener
		//      one state in 100 ticks
		static long time_from_last_send = 0;
		time_from_last_send += i_elapsed_time;
		if (time_from_last_send > MS_FOR_TICK * 100)
			{
			SendControllerStates(*ip_owner->GetContext(), *serverMain.GetConnection());
			time_from_last_send = 0;

			if (ip_owner->GetGameController().IsGameOver())
				SendEndGameMessage(*serverMain.GetConnection(), true);
			}
		}

	void PlayState::Exit(GameState* ip_owner)
		{
		printf("Exit PlayState\n");
		}
	
	void HandleCommand(SDK::GameCore::CommandManager& i_command_manager, IController* ip_controller, unsigned char* ip_packet, size_t i_bytes_read)
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

	void PlayState::HoldPacket(GameState* ip_owner, unsigned char* ip_packet, size_t i_bytes_read)
		{
		ServerMain& serverMain = ServerMain::GetInstance();
		Network::PacketType packet_type = (Network::PacketType)FromChar<int>(reinterpret_cast<char*>(ip_packet));
		switch (packet_type)
			{
			case Network::PacketType::PT_EndGame:
				serverMain.Stop();
				return;
				break;
			case Network::PacketType::PT_Command:
				{
				int controllers_mask = serverMain.GetConnection()->GetAddress().GetAddress();
				auto& controllers = ip_owner->GetContext()->GetControllers();
				auto it = std::find_if(controllers.begin(), controllers.end(), [controllers_mask](const GameContext::ControllerInformation& controller_info)
					{
					return controller_info.mp_controller->GetMask() == controllers_mask;
					});
				if (it != controllers.end())
					HandleCommand(serverMain.GetMetaFactory().GetCommandManager(), it->mp_controller.get(), ip_packet, i_bytes_read);
				return;
				}
			case Network::PacketType::PT_Selection:
				{
				int controllers_mask = serverMain.GetConnection()->GetAddress().GetAddress();
				auto& controllers = ip_owner->GetContext()->GetControllers();
				auto it = std::find_if(controllers.begin(), controllers.end(), [controllers_mask](const GameContext::ControllerInformation& controller_info)
					{
					return controller_info.mp_controller->GetMask() == controllers_mask;
					});
				if (it != controllers.end())
					it->mp_controller->HoldPacket(ip_packet, i_bytes_read);
				return;
				}
				break;
			}
		}

	///////////////////////////////////////////////////////////////////////////
	// PlayerDecisionState

	PlayerDecisionState::PlayerDecisionState()
		: m_player_make_decision(false)
		{

		}

	void PlayerDecisionState::Enter(GameState* ip_owner)
		{
		printf("Enter PlayerDecisionState\n");
		m_player_make_decision = false;
		SDK::RequestedAction requested_action = static_cast<SDK::RequestedAction>(ip_owner->GetGameController().GetGameInfo().m_requested_action);
		switch (requested_action)
			{
			case SDK::RequestedAction::ShowInformation:
				{
				ServerMain& serverMain = ServerMain::GetInstance();
				auto p_connection = serverMain.GetConnection();
				char buffer[sizeof(Network::PacketType)+sizeof(int)];
				ToChar(Network::PacketType::PT_ShowInformation, buffer, sizeof(Network::PacketType));
				ToChar(ip_owner->GetGameController().GetGameInfo().m_dialog_id, &buffer[sizeof(Network::PacketType)], sizeof(int));
				p_connection->SendPacket(buffer, sizeof(buffer));
				}
				break;
			default:
				m_player_make_decision = true;
				break;
			}
		}

	void PlayerDecisionState::Execute(GameState* ip_owner, long i_elapsed_time)
		{
		ServerMain& serverMain = ServerMain::GetInstance();

		static std::unique_ptr<unsigned char[]> packet(new unsigned char[PACKET_SIZE]);
		while (true)
			{
			int bytes_read = serverMain.GetConnection()->ReceivePacket(&packet[0], PACKET_SIZE);
			if (0 == bytes_read)
				break;

			HoldPacket(ip_owner, &packet[0], bytes_read);
			}

		if (m_player_make_decision)
			ip_owner->GetGameController().RequestStateChange(-1);
		}

	void PlayerDecisionState::Exit(GameState* ip_owner)
		{
		ip_owner->GetGameController().GetGameInfo().m_requested_action = static_cast<int>(SDK::RequestedAction::None);
		ip_owner->GetGameController().GetGameInfo().m_dialog_id = -1;
		printf("Exit PlayerDecisionState\n");
		}

	void PlayerDecisionState::HoldPacket(GameState* ip_owner, unsigned char* ip_packet, size_t i_bytes_read)
		{
		ServerMain& serverMain = ServerMain::GetInstance();
		Network::PacketType packet_type = (Network::PacketType)FromChar<int>(reinterpret_cast<char*>(ip_packet));
		switch (packet_type)
			{
			case Network::PacketType::PT_Achived:
				m_player_make_decision = true;
				break;
			}
		}

	} // Slavs