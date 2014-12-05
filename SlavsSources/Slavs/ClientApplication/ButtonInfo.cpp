#include "stdafx.h"

#include "ButtonInfo.h"

namespace UI
  {

  ButtonInfo::ButtonInfo(int i_ui_id,
    const std::string& i_string_id,
    const std::string& i_name,
    const std::string& i_button_type,
    const std::string& i_tooltip /* = std::string()*/,
		bool i_dsplay_text /* = true*/)
    : m_ui_id(i_ui_id)
    , m_string_id(i_string_id)
    , m_display_name(i_name)
    , m_button_type(i_button_type)
    , m_tooltip(i_tooltip)
		, m_display_text(Uri_DISPLAY_IDN_HOST)
    {}

  ButtonInfo::ButtonInfo(const std::string& i_string_id)
    : m_ui_id(-1)
    , m_string_id(i_string_id)
    , m_display_name()
    , m_button_type()
    , m_tooltip()
		, m_display_text(true)
    {}

  bool ButtonInfo::IsValid() const
    {
    return m_ui_id != -1;
    }

  ///////////////////////////////////////////////////////////////////////////
  // Comparers
  ///////////////////////////////////////////////////////////////////////////
  // ID

  ButtonInfo::Comparer_Id::Comparer_Id(int i_command_id)
    : m_command_id(i_command_id)
    {}

  bool ButtonInfo::Comparer_Id::operator() (const ButtonInfo& i_command) const
    {
    return i_command.m_ui_id == m_command_id;
    }

  ///////////////////////////////////////////////////////////////////////////
  // String id
  ButtonInfo::Comparer_StringId::Comparer_StringId(const std::string& i_string_id)
    : m_string_id(i_string_id)
    {}

  bool ButtonInfo::Comparer_StringId::operator () (const ButtonInfo& i_command) const
    {
    return i_command.m_string_id == m_string_id;
    }

  ///////////////////////////////////////////////////////////////////////////

  } // UI