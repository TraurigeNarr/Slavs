#ifndef __INFORMATIONSCREEN_H__
#define __INFORMATIONSCREEN_H__

#include "CeguiScreenBase.h"

#include "Dialog.h"

namespace UI
	{

	class InformationScreen : public CeguiScreenBase
		{
		private:
			CEGUI::Window*	mp_information_view;

		private:
			virtual void Initialize() override;

			void ShowInformation(int i_id);
			void ShowDialog(int i_id);

			bool ButtonPressed(const CEGUI::EventArgs& i_arguments);

		public:
			InformationScreen(ScreenManager& i_screen_manager);
			virtual ~InformationScreen();

			virtual void Update(long i_elapsed_time) override;
		};

	} // UI

#endif