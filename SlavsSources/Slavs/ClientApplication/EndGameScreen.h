#ifndef __ENDGAMESCREEN_H__
#define __ENDGAMESCREEN_H__

#include "CeguiScreenBase.h"

namespace UI
	{

	class EndGameScreen : public CeguiScreenBase
		{
		private:
			bool m_show_information;

		private:
			virtual void Initialize() override;

		public:
			EndGameScreen(ScreenManager& i_screen_manager);
			virtual ~EndGameScreen();

			virtual void Update(long i_elapsed_time) override;
		};

	} // UI

#endif