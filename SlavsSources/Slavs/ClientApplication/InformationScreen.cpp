#include "stdafx.h"
#include "InformationScreen.h"

#include "Application.h"
#include "GameStateMessageProvider.h"
#include "InformationMessageProvider.h"
#include "UISettings.h"


namespace UI
	{
	
	enum class InformationScreenButton
		{
		Ok,
		Cancel,
		Ignore,
		};

	InformationScreen::InformationScreen(ScreenManager& i_screen_manager)
		: CeguiScreenBase(i_screen_manager)
		, mp_information_view(nullptr)
		{

		}

	InformationScreen::~InformationScreen()
		{
		if (mp_information_view && mp_root_window)
			{
			mp_root_window->removeChild(mp_information_view);
			CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
			wmgr.destroyWindow(mp_information_view);
			}
		}

	void InformationScreen::ShowInformation(int i_id)
		{
		auto dlg = ClientGame::appInstance.GetUISettings().GetDialog(i_id);
		
		CEGUI::DefaultWindow* p_image = static_cast<CEGUI::DefaultWindow*>(mp_information_view->getChild("Background"));
		p_image->setProperty("Image", dlg.GetImage());
		//p_image->getChild("InformationText")->setText(dlg.GetText());

		CEGUI::Window* p_text = CEGUI::WindowManager::getSingletonPtr()->createWindow("SlavsLook/StaticText");

		p_text->setText(dlg.GetText());
		p_text->setSize(CEGUI::USize(CEGUI::UDim(0.5, .0f), CEGUI::UDim(0.5, .0f)));
		p_text->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2f, .0f), CEGUI::UDim(0.2f, .0f)));
		mp_information_view->addChild(p_text);

		mp_information_view->setText(dlg.GetCaption());
		mp_information_view->setVisible(true);
		}

	void InformationScreen::ShowDialog(int i_id)
		{
		throw std::runtime_error("Not implemented");
		}

	void InformationScreen::Initialize()
		{
		CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
		mp_information_view = wmgr.loadLayoutFromFile("InformationView.layout");
		mp_root_window->addChild(mp_information_view);
		mp_information_view->setVisible(false);

		try
			{
			// Ok
			CEGUI::Window* p_ok_button = mp_information_view->getChild("Background/OkButton");
			if (p_ok_button != nullptr)
				{
				p_ok_button->setID(static_cast<int>(InformationScreenButton::Ok));
				p_ok_button->subscribeEvent(CEGUI::PushButton::EventClicked,
					CEGUI::Event::Subscriber(&InformationScreen::ButtonPressed, this));
				}
			}
		catch (CEGUI::UnknownObjectException&)
			{
			assert(false);
			}
		}

	void InformationScreen::Update(long i_elapsed_time)
		{
		if (mp_information_view->isVisible())
			return;

		auto p_message_provider = GetMessageProvider<ClientStates::GameStateMessageProvider>();
		if (p_message_provider == nullptr)
			return;

		auto p_information = p_message_provider->GetProvider<ClientStates::InformationMessageProvider>();

		if (p_information && p_information->IsValid())
			return;

		auto window = p_information->GetNextInformationWindow();
		if (window.m_type == WindowType::None)
			return;

		if (window.m_type == WindowType::Information)
			ShowInformation(window.m_id);
		else if (window.m_type == WindowType::Dialog)
			ShowDialog(window.m_id);

		p_information->Validate();
		}

	bool InformationScreen::ButtonPressed(const CEGUI::EventArgs& i_arguments)
		{
		const CEGUI::WindowEventArgs& window_arguments = static_cast<const CEGUI::WindowEventArgs&>(i_arguments);
		InformationScreenButton button_id = static_cast<InformationScreenButton>(window_arguments.window->getID());

		switch (button_id)
			{
			case InformationScreenButton::Ok:
				mp_information_view->setVisible(false);
				break;
			}

		return true;
		}

	} // UI