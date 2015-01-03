#include "stdafx.h"
#include "EndGameScreen.h"

#include "GameStateMessageProvider.h"
#include "EndGameInformationProvider.h"
#include "UISettings.h"

#include "ButtonID.h"

namespace UI
	{

	EndGameScreen::EndGameScreen(ScreenManager& i_screen_manager)
		: CeguiScreenBase(i_screen_manager)
		, m_show_information(false)
		{		}

	EndGameScreen::~EndGameScreen()
		{		}

	void EndGameScreen::Initialize()
		{
		}

	void EndGameScreen::Update(long i_elapsed_time)
		{
		if (m_show_information)
			return;

		auto p_message_provider = GetMessageProvider<ClientStates::GameStateMessageProvider>();
		if (p_message_provider == nullptr)
			return;

		auto p_information = p_message_provider->GetProvider<ClientStates::EndGameInformationProvider>();

		if (p_information && p_information->IsValid())
			return;

		if (p_information->EndGame())
			{
			CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
			auto p_root = wmgr.loadLayoutFromFile("EndGameView.layout");

			_TryAttachButtonHandler(*p_root, "Background/Menu", static_cast<CEGUI::uint>(ButtonID::BI_BACK_TO_MENU));


			CEGUI::Window* p_text = CEGUI::WindowManager::getSingletonPtr()->createWindow("SlavsLook/StaticText");

			p_text->setText(p_information->Win() ? "You win" : "You loose");
			p_text->setSize(CEGUI::USize(CEGUI::UDim(0.5, .0f), CEGUI::UDim(0.5, .0f)));
			p_text->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2f, .0f), CEGUI::UDim(0.2f, .0f)));
			p_root->addChild(p_text);

			p_root->setText("Win message");

			mp_root_window->addChild(p_root);
			m_show_information = true;
			}
		}

	} // UI