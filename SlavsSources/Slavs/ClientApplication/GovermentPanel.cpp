#include "stdafx.h"

#include "GovermentPanel.h"

#include "EconomySystemMessageProvider.h"
#include "GameStateMessageProvider.h"
#include "GovermentMessageProvider.h"

#include "ScreenManager.h"
#include "ButtonID.h"


namespace
  {

  enum class ButtonState
    {
    Good,
    Intermediate,
    Bad
    };


  void _SetButtonImages(CEGUI::Window& i_button, const ButtonState& i_state, std::string i_button_image_name)
    {
    switch (i_state)
      {
      case ButtonState::Good:
        i_button_image_name += "_Good";
        break;
      case ButtonState::Bad:
        i_button_image_name += "_Bad";
        break;
      case ButtonState::Intermediate:
        i_button_image_name += "_Intermediate";
        break;
      }
    i_button.setProperty("NormalImage", i_button_image_name);
    i_button.setProperty("HoverImage", i_button_image_name);
    i_button.setProperty("PushedImage", i_button_image_name);
    i_button.setProperty("DisabledImage", i_button_image_name);
    }

  }

namespace UI
  {

  GovermentPanel::GovermentPanel(ScreenManager& i_screen_manager)
    : CeguiScreenBase(i_screen_manager)
    {    }

  GovermentPanel::~GovermentPanel()
    {    }

  void GovermentPanel::Initialize()
    {
    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
    
    try
      {
      m_plugin_buttons[ButtonID::BI_ECONOMY_PLUGIN_STATUS] = mp_root_window->getChild("EconomyPluginStatus");
      _SetButtonImages (*m_plugin_buttons[ButtonID::BI_ECONOMY_PLUGIN_STATUS], ButtonState::Good, "SlavsGameScreen/EconomyPluginStatus");
      }
    catch (CEGUI::UnknownObjectException&)
      {      }

    _TryAttachButtonHandler(*mp_root_window, "EconomyPluginStatus", static_cast<CEGUI::uint>(ButtonID::BI_ECONOMY_PLUGIN_STATUS));
    }

  void GovermentPanel::Update (long i_elapsed_time)
    {
    auto p_message_provider = GetMessageProvider<ClientStates::GameStateMessageProvider>();
    if (p_message_provider == nullptr || p_message_provider->IsValid())
      return;

    auto p_economy = p_message_provider->GetProvider<ClientStates::GovermentMessageProvider>()->GetProvider<ClientStates::EconomySystemMessageProvider>();

    if (p_economy)
      {
      switch (p_economy->GetEconomySystemState())
        {
        case ClientStates::ManagerState::Good:
          _SetButtonImages (*m_plugin_buttons[ButtonID::BI_ECONOMY_PLUGIN_STATUS], ButtonState::Good, "SlavsGameScreen/EconomyPluginStatus");
          break;
        case ClientStates::ManagerState::Intermediate:
          _SetButtonImages (*m_plugin_buttons[ButtonID::BI_ECONOMY_PLUGIN_STATUS], ButtonState::Intermediate, "SlavsGameScreen/EconomyPluginStatus");
          break;
        case ClientStates::ManagerState::Bad:
          _SetButtonImages (*m_plugin_buttons[ButtonID::BI_ECONOMY_PLUGIN_STATUS], ButtonState::Bad, "SlavsGameScreen/EconomyPluginStatus");
          break;
        }
      }

    p_message_provider->Validate();
    }

  } // UI