#ifndef __ENDGAMEINFORMATIONPROVIDER_H__
#define __ENDGAMEINFORMATIONPROVIDER_H__

#include "GameStateBaseMessageProvider.h"

namespace ClientStates
	{

	class EndGameInformationProvider : public GameStateBaseMessageProvider
		{
		private:
			bool m_win;
			bool m_end_game;

		public:
			EndGameInformationProvider(GameState& i_state);
			virtual ~EndGameInformationProvider();

			virtual bool  HandlePacket(const Network::Packet& i_packet) override;

			bool EndGame() const { return m_end_game; }
			bool Win() const { return m_win; }
		};

	} //ClientStates

#endif