#include "stdafx.h"

#include "InformationMessageProvider.h"

#include "GameState.h"

#include <Network/Packet.h>

#include <Common/Utilities/TemplateFunctions.h>

namespace ClientStates
	{

	InformationMessageProvider::InformationMessageProvider(GameState& i_state)
		: GameStateBaseMessageProvider(i_state)
		{
		}

	InformationMessageProvider::~InformationMessageProvider()
		{    }

	void InformationMessageProvider::AddInformation(UI::WindowType i_type, int i_id)
		{
		m_information_to_show.push_back(UI::WindowInformation(i_type, i_id, std::string()));
		__super::Invalidate();
		}

	void InformationMessageProvider::Validate()
		{
		if (!m_information_to_show.empty())
			m_information_to_show.pop_back();

		if (m_information_to_show.empty())
			__super::Validate();
		}

	UI::WindowInformation	InformationMessageProvider::GetNextInformationWindow() const
		{
		if (m_information_to_show.empty())
			return UI::WindowInformation();

		return m_information_to_show.back();
		}

	bool InformationMessageProvider::HandlePacket(const Network::Packet& i_packet)
		{
		if (i_packet.m_packet != Network::PacketType::PT_ShowDialog && i_packet.m_packet != Network::PacketType::PT_ShowInformation)
			return false;

		UI::WindowType type = UI::WindowType::None;
		if (i_packet.m_packet == Network::PacketType::PT_ShowInformation)
			type = UI::WindowType::Information;
		else if (i_packet.m_packet == Network::PacketType::PT_ShowDialog)
			type = UI::WindowType::Dialog;

		auto id = ConvertTo<int>(i_packet.GetPointerToCurrentConst());
		i_packet.IncreaseOffset(sizeof(int));
		AddInformation(type, id);

		return true;
		}

	} // ClientStates