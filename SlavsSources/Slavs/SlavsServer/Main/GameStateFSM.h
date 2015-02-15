#ifndef __GAMESTATEFSM_H__
#define __GAMESTATEFSM_H__

#include <Patterns/State.h>

#include <memory>

class TimeController;

namespace Slavs
	{
	class GameState;

	class PlayState : public State < GameState, long >
		{
		private:
			void HoldPacket(GameState* ip_owner, unsigned char* ip_packet, size_t i_bytes_read);

		public:
			PlayState();
			virtual void Enter(GameState* ip_owner) override;
			virtual void Execute(GameState* ip_owner, long i_elapsed_time) override;
			virtual void Exit(GameState* ip_owner) override;
		};

	class PlayerDecisionState : public State < GameState, long >
		{
		private:
			bool m_player_make_decision;

		private:
			void HoldPacket(GameState* ip_owner, unsigned char* ip_packet, size_t i_bytes_read);

		public:
			PlayerDecisionState();
			virtual void Enter(GameState* ip_owner) override;
			virtual void Execute(GameState* ip_owner, long i_elapsed_time) override;
			virtual void Exit(GameState* ip_owner) override;
		};

	} // Slavs

#endif