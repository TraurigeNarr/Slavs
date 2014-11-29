#include "stdafx.h"

#include "CommandsPanel.h"

#include "ElementsBox.h"
#include "CeguiUtilities.h"

namespace Dummy
  {
  enum class CommandType
    {
    None = -1,
    Building_Production = 0,
    Building_Society
    };

  // get it from server
  struct CommandInfo
    {
    CommandType m_type;
    int         m_id;
    std::string m_name;
    std::string m_tooltip;

    CommandInfo(CommandType i_type, int i_id, const std::string& i_name, const std::string& i_tooltip = std::string())
      : m_type(i_type)
      , m_id(i_id)
      , m_name(i_name)
      , m_tooltip(i_tooltip)
      {}
    };

  std::vector<CommandInfo> m_commands;

  void CreateDummyCommands()
    {
    m_commands.push_back(CommandInfo(CommandType::Building_Production, 0, "Store", "Build Store"));
    m_commands.push_back(CommandInfo(CommandType::Building_Production, 1, "Mill", "Build Mill"));

    m_commands.push_back(CommandInfo(CommandType::Building_Society, 2, "House", "Build House"));
    m_commands.push_back(CommandInfo(CommandType::Building_Society, 3, "Hizhina", "Build Hizhina"));
    }

  std::string GetNameFromType(CommandType i_type)
    {
    switch (i_type)
      {
      case CommandType::Building_Production:
        return "Production";
      case CommandType::Building_Society:
        return "Society";
      }
    throw std::exception();
    }

  bool created = false;
  }

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
    if (!Dummy::created)
      {
      Dummy::created = true;
      Dummy::CreateDummyCommands();
      }
    }

  CommandsPanel::~CommandsPanel()
    {}

  void CommandsPanel::Initialize()
    {
    // TODO: move this to GameScreenMain
    //CEGUI::ImageManager::getSingleton().loadImageset("SlavsGameCommands.imageset");

    std::vector<std::pair<Dummy::CommandType, int>> categories;
    using namespace Dummy;
    CommandType current_type = m_commands[0].m_type;
    ElementsBox* p_box = nullptr;
    for (CommandInfo& info : m_commands)
      {
      if (info.m_type != current_type)
        {
        p_box = nullptr;
        current_type = info.m_type;
        }
      if (p_box == nullptr)
        p_box = m_panel.AddBox(GetNameFromType(current_type));

      p_box->AddCommand(info.m_name, info.m_id);
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
    const float button_size = 1.f / shown_boxes.size();
    for (size_t i = 0; i < shown_boxes.size(); ++i)
      {
      const ElementsBox* p_box = shown_boxes[i];
      CEGUI::Window* p_box_button = CEGUI::WindowManager::getSingletonPtr()->createWindow("SlavsLook/Button");
      p_box_button->setID(p_box->GetId());
      p_box_button->setText(p_box->GetName());
      p_box_button->setSize(CEGUI::USize(CEGUI::UDim(button_size, 0.f), CEGUI::UDim(1.f, 0.f)));
      p_box_button->setPosition(CEGUI::UVector2(CEGUI::UDim(i*button_size, 0.f), CEGUI::UDim(0.f, 0.f)));

      p_box_button->subscribeEvent(CEGUI::PushButton::EventClicked,
        CEGUI::Event::Subscriber(&CommandsPanel::ListButtonPressed, this));

      p_list_root->addChild(p_box_button);
      }
    }

  void CommandsPanel::RedrawCommands()
    {
    auto p_current_box = m_panel.GetCurrentBox();
    auto p_commands_root = mp_root_window->getChild("CommandsPanel.Commands");
    CeguiUtilities::ClearWindow(*p_commands_root);

    const auto& shown_commands = p_current_box->GetCurrentShownCommands();
    const float button_size = 1.f / shown_commands.size();
    for (size_t i = 0; i < shown_commands.size(); ++i)
      {
      const ElementsBox::Command& command = shown_commands[i];
      CEGUI::Window* p_box_button = CEGUI::WindowManager::getSingletonPtr()->createWindow("SlavsLook/Button");

      p_box_button->setID(command.m_id);
      p_box_button->setText(command.m_name);
      p_box_button->setSize(CEGUI::USize(CEGUI::UDim(button_size, 0.f), CEGUI::UDim(1.f, 0.f)));
      p_box_button->setPosition(CEGUI::UVector2(CEGUI::UDim(i*button_size, 0.f), CEGUI::UDim(0.f, 0.f)));

      p_box_button->subscribeEvent(CEGUI::PushButton::EventClicked,
        CEGUI::Event::Subscriber(&CeguiScreenBase::CommandButtonPressedHandler, static_cast<CeguiScreenBase*>(this)));

      p_commands_root->addChild(p_box_button);
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