#include "stdafx.h"

#include "CommandInfo.h"

namespace UI
  {

  CommandInfo::CommandInfo()
    {

    }

  ///////////////////////////////////////////////////////////////////////////
  // Comparers
  ///////////////////////////////////////////////////////////////////////////
  // ID

  CommandInfo::Comparer_Id::Comparer_Id(int i_command_id)
    : m_command_id(i_command_id)
    {}

  bool CommandInfo::Comparer_Id::operator() (const CommandInfo& i_command) const
    {
    return i_command.m_id == m_command_id;
    }

  ///////////////////////////////////////////////////////////////////////////
  // String id
  CommandInfo::Comparer_StringId::Comparer_StringId(const std::string& i_string_id)
    : m_string_id(i_string_id)
    {}

  bool CommandInfo::Comparer_StringId::operator () (const CommandInfo& i_command) const
    {
    return i_command.m_string_id == m_string_id;
    }

  ///////////////////////////////////////////////////////////////////////////

  } // UI