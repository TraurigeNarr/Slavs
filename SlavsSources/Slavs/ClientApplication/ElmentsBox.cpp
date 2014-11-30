#include "stdafx.h"

#include "ElementsBox.h"

#include "ElementsPanel.h"

namespace UI
  {
  ElementsBox::ElementsBox(ElementsPanel& i_owner_panel, int i_type_id, int i_id)
    : m_owner_panel(i_owner_panel)
    , m_type_id(i_type_id)
    , m_id(i_id)
    {

    }
  ElementsBox::~ElementsBox()
    {}

  void ElementsBox::AddCommand(const CommandInfo& i_info)
    {
#ifdef _DEBUG
    // it should be one command with such id
    auto it = std::find_if(m_commands.begin(), m_commands.end(), CommandInfo::Comparer_Id(i_info.m_id));
    if (it != m_commands.end())
      assert(false);
#endif

    m_commands.push_back(i_info);
    }

  void ElementsBox::RemoveCommand(int i_command_id)
    {    
    auto it = std::remove_if(m_commands.begin(), m_commands.end(), CommandInfo::Comparer_Id(i_command_id));
    m_commands.erase(it, m_commands.end());
    }

  int ElementsBox::GetCommandId(const std::string& i_string_id) const
    {
    auto it = std::find_if(m_commands.begin(), m_commands.end(), CommandInfo::Comparer_StringId(i_string_id));
    if (it == m_commands.end())
      return -1;

    return it->m_id;
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

  const std::vector<CommandInfo>& ElementsBox::GetCurrentShownCommands() const
    {
    return m_commands;
    }

  } // UI