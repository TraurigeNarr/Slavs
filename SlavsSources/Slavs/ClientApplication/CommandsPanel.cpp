#include "stdafx.h"

#include "CommandsPanel.h"

#include "ElementsBox.h"
#include "CeguiUtilities.h"

#include "UISettings.h"

#include "Application.h"

namespace UI
  {

  enum class ShiftButtonId
    {
    ListLeft,
    ListRight,
    CommandsLeft,
    CommandsRight
    };

  CommandsPanel::CommandsPanel(ScreenManager& i_screen_manager)
    : CeguiScreenBase(i_screen_manager)
    {
    }

  CommandsPanel::~CommandsPanel()
    {}

  void CommandsPanel::Initialize()
    {
    UISettings& ui_settings = ClientGame::appInstance.GetUISettings();
    const CommandsInfrmation& commands = ui_settings.GetServerCommands();
    if (commands.size() == 0)
      return;
    int current_type = commands[0].m_type_id;
    ElementsBox* p_box = nullptr;
    for (const CommandInfo& info : commands)
      {
      if (info.m_type_id != current_type)
        p_box = nullptr;

      if (p_box == nullptr)
        {
        p_box = m_panel.AddBox(info.m_type_id);
        current_type = info.m_type_id;
        }

      p_box->AddCommand(info);
      }

    RedrawAll();

    auto p_list_left = mp_root_window->getChild("CommandsPanel.List.Left");
    p_list_left->setID(static_cast<CEGUI::uint>(ShiftButtonId::ListLeft));
    p_list_left->subscribeEvent(CEGUI::PushButton::EventClicked,
      CEGUI::Event::Subscriber(&CommandsPanel::ShiftButtonPressed, this));

    auto p_list_right = mp_root_window->getChild("CommandsPanel.List.Right");
    p_list_right->setID(static_cast<CEGUI::uint>(ShiftButtonId::ListRight));
    p_list_right->subscribeEvent(CEGUI::PushButton::EventClicked,
      CEGUI::Event::Subscriber(&CommandsPanel::ShiftButtonPressed, this));

    auto p_commands_left = mp_root_window->getChild("CommandsPanel.Commands.Left");
    p_commands_left->setID(static_cast<CEGUI::uint>(ShiftButtonId::CommandsLeft));
    p_commands_left->subscribeEvent(CEGUI::PushButton::EventClicked,
      CEGUI::Event::Subscriber(&CommandsPanel::ShiftButtonPressed, this));

    auto p_commands_right = mp_root_window->getChild("CommandsPanel.Commands.Right");
    p_commands_right->setID(static_cast<CEGUI::uint>(ShiftButtonId::CommandsRight));
    p_commands_right->subscribeEvent(CEGUI::PushButton::EventClicked,
      CEGUI::Event::Subscriber(&CommandsPanel::ShiftButtonPressed, this));
    }

  void CommandsPanel::Update(long i_elapsed_time)
    {
    
    }

  void CommandsPanel::RedrawList()
    {
    auto p_list_root = mp_root_window->getChild("CommandsPanel.List");
    CeguiUtilities::ClearWindow(*p_list_root);

    const auto& shown_boxes = m_panel.GetCurrentShownBoxes();
    if (shown_boxes.size() == 0)
      return;
    const float button_size = 1.f / shown_boxes.size();
    for (size_t i = 0; i < shown_boxes.size(); ++i)
      {
      const ElementsBox* p_box = shown_boxes[i];
      try
        {
        const ButtonInfo& button_info = ClientGame::appInstance.GetUISettings().GetButtonInfoFromType(p_box->GetTypeId());

        CEGUI::Window* p_box_button = CEGUI::WindowManager::getSingletonPtr()->createWindow(button_info.m_button_type);
        p_box_button->setID(p_box->GetId());
        p_box_button->setText(button_info.m_display_name);
        p_box_button->setTooltipText(button_info.m_tooltip);

        p_box_button->setSize(CEGUI::USize(CEGUI::UDim(button_size, 0.f), CEGUI::UDim(1.f, 0.f)));
        p_box_button->setPosition(CEGUI::UVector2(CEGUI::UDim(i*button_size, 0.f), CEGUI::UDim(0.f, 0.f)));

        p_box_button->subscribeEvent(CEGUI::PushButton::EventClicked,
          CEGUI::Event::Subscriber(&CommandsPanel::ListButtonPressed, this));

        p_list_root->addChild(p_box_button);
        }
      catch (std::logic_error&)
        {
        // TODO trace
        continue;
        }
      }
    }

  void CommandsPanel::RedrawCommands()
    {
    auto p_current_box = m_panel.GetCurrentBox();

    if (p_current_box == nullptr)
      return;

    auto p_commands_root = mp_root_window->getChild("CommandsPanel.Commands");
    CeguiUtilities::ClearWindow(*p_commands_root);

    const auto& shown_commands = p_current_box->GetCurrentShownCommands();
    if (shown_commands.size() == 0)
      return;
    const float button_size = 1.f / shown_commands.size();
    for (size_t i = 0; i < shown_commands.size(); ++i)
      {
      const CommandInfo& command = shown_commands[i];

      try
        {
        const ButtonInfo& button_info = ClientGame::appInstance.GetUISettings().GetButtonInfo(command.m_ui_id);

        CEGUI::Window* p_box_button = CEGUI::WindowManager::getSingletonPtr()->createWindow(button_info.m_button_type);

        p_box_button->setID(command.m_id);
				if (button_info.m_display_text)
					p_box_button->setText(button_info.m_display_name);
        p_box_button->setTooltipText(button_info.m_tooltip);

        p_box_button->setSize(CEGUI::USize(CEGUI::UDim(button_size, 0.f), CEGUI::UDim(1.f, 0.f)));
        p_box_button->setPosition(CEGUI::UVector2(CEGUI::UDim(i*button_size, 0.f), CEGUI::UDim(0.f, 0.f)));

        p_box_button->subscribeEvent(CEGUI::PushButton::EventClicked,
          CEGUI::Event::Subscriber(&CeguiScreenBase::CommandButtonPressedHandler, static_cast<CeguiScreenBase*>(this)));

        p_commands_root->addChild(p_box_button);
        }
      catch (std::logic_error&)
        {
        // TODO trace
        continue;
        }
      }
    }

  void CommandsPanel::RedrawAll()
    {
    RedrawList();
    RedrawCommands();
    }

  void CommandsPanel::ShiftListLeft()
    {
    if (m_panel.ShiftLeft())
      RedrawList();
    }

  void CommandsPanel::ShiftListRight()
    {
    if (m_panel.ShiftRight())
      RedrawList();
    }

  void CommandsPanel::ShiftCommandsLeft()
    {
    
    }

  void CommandsPanel::ShiftCommandsRight()
    {
    
    }

  bool CommandsPanel::ShiftButtonPressed(const CEGUI::EventArgs& i_arguments)
    {
    const CEGUI::WindowEventArgs& window_arguments = static_cast<const CEGUI::WindowEventArgs&>(i_arguments);
    ShiftButtonId button_id = static_cast<ShiftButtonId>(window_arguments.window->getID());
    switch (button_id)
      {
      case ShiftButtonId::ListLeft:
        {
        ShiftListLeft();
        return true;
        }
        break;
      case ShiftButtonId::ListRight:
        {
        ShiftListRight();
        return true;
        }
        break;
      case ShiftButtonId::CommandsLeft:
        {
        ShiftCommandsLeft();
        return true;
        }
        break;
      case ShiftButtonId::CommandsRight:
        {
        ShiftCommandsRight();
        return true;
        }
        break;
      }

    return false;
    }

  bool CommandsPanel::ListButtonPressed(const CEGUI::EventArgs& i_arguments)
    {
    const CEGUI::WindowEventArgs& window_arguments = static_cast<const CEGUI::WindowEventArgs&>(i_arguments);
    const int list_button_id = window_arguments.window->getID();
    
    if (m_panel.SetCurrentBox(list_button_id))
      RedrawCommands();

    return false;
    }

  } // UI