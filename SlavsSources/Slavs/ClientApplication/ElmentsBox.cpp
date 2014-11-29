#include "stdafx.h"

#include "ElementsBox.h"

#include "ElementsPanel.h"

namespace
  {

  struct CommandComparer_Id
    {
    const int m_command_id;
    CommandComparer_Id(int i_command_id)
      : m_command_id(i_command_id)
      {}

    bool operator () (const UI::ElementsBox::Command& i_command)
      {
      return i_command.m_id == m_command_id;
      }
    };

  struct CommandComparer_Name
    {
    const std::string m_command_name;
    CommandComparer_Name(const std::string& i_command_name)
      : m_command_name(i_command_name)
      {}

    bool operator () (const UI::ElementsBox::Command& i_command)
      {
      return i_command.m_name == m_command_name;
      }
    };

  }

namespace UI
  {
  ElementsBox::ElementsBox(ElementsPanel& i_owner_panel, const std::string& i_name, int i_id)
    : m_owner_panel(i_owner_panel)
    , m_name(i_name)
    , m_id(i_id)
    {

    }
  ElementsBox::~ElementsBox()
    {}

  void ElementsBox::AddCommand(const std::string& i_display_name, int i_command_id)
    {
#ifdef _DEBUG
    // it should be one command with such id
    auto it = std::find_if(m_commands.begin(), m_commands.end(), CommandComparer_Id(i_command_id));
    if (it != m_commands.end())
      assert(false);
#endif

    m_commands.push_back(Command(i_command_id, i_display_name));
    }

  void ElementsBox::RemoveCommand(int i_command_id)
    {    
    auto it = std::remove_if(m_commands.begin(), m_commands.end(), CommandComparer_Id(i_command_id));
    m_commands.erase(it, m_commands.end());
    }

  int ElementsBox::GetCommandId(const std::string& i_display_name) const
    {
    auto it = std::find_if(m_commands.begin(), m_commands.end(), CommandComparer_Name(i_display_name));
    if (it == m_commands.end())
      return -1;

    return it->m_id;
    }

  std::string ElementsBox::GetCommandName(int i_command_id) const
    {
    auto it = std::find_if(m_commands.begin(), m_commands.end(), CommandComparer_Id(i_command_id));
    if (it == m_commands.end())
      return std::string();

    return it->m_name;
    }

  bool ElementsBox::CanShiftRght() const
    {
    return true;
    }

  bool ElementsBox::CanShiftLeft() const
    {
    return true;
    }

  bool ElementsBox::ShiftRight()
    {
    return true;
    }

  bool ElementsBox::ShiftLeft()
    {
    return true;
    }

  const std::vector<ElementsBox::Command>& ElementsBox::GetCurrentShownCommands() const
    {
    return m_commands;
    }

  } // UI