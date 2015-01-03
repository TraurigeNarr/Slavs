#include "stdafx.h"

#include "EndGameInformationProvider.h"

#include <Network/Packet.h>

#include <Common/Utilities/TemplateFunctions.h>

namespace ClientStates
	{

	EndGameInformationProvider::EndGameInformationProvider(GameState& i_state)
		: GameStateBaseMessageProvider(i_state)
		, m_win(false)
		, m_end_game(false)
		{

		}

	EndGameInformationProvider::~EndGameInformationProvider()
		{

		}

	bool  EndGameInformationProvider::HandlePacket(const Network::Packet& i_packet)
		{
		if (i_packet.m_packet != Network::PacketType::PT_EndGame)
			return false;

		m_win = ConvertTo<bool>(i_packet.GetPointerToCurrentConst());
		m_end_game = true;
		}

	} // ClientStates