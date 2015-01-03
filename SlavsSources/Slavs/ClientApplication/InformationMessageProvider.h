#ifndef __INFORMATIONMESSAGEPROVIDER_H__
#define __INFORMATIONMESSAGEPROVIDER_H__

#include "GameStateBaseMessageProvider.h"
#include "WindowInformation.h"

namespace ClientStates
	{

	class InformationMessageProvider : public GameStateBaseMessageProvider
		{
		private:
			std::vector<UI::WindowInformation> m_information_to_show;

		public:
			InformationMessageProvider(GameState& i_state);
			virtual ~InformationMessageProvider();

			void					AddInformation(UI::WindowType i_type, int i_id);

			virtual void  Validate() override;
			virtual bool  HandlePacket(const Network::Packet& i_packet) override;

			UI::WindowInformation	GetNextInformationWindow() const;
		};

	} // ClientStates

#endif