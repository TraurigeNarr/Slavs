#ifndef __BUTININFO_H__
#define __BUTININFO_H__

namespace UI
  {

  struct ButtonInfo
    {
    int         m_ui_id;
    std::string m_string_id;
    std::string m_display_name;
    std::string m_tooltip;
    std::string m_button_type;
		bool				m_display_text;

    ButtonInfo(int i_ui_id,
              const std::string& i_string_id,
              const std::string& i_name,
              const std::string& i_button_type,
              const std::string& i_tooltip = std::string(),
							bool i_display_text = true);
    
    ButtonInfo(const std::string& i_string_id);

    bool IsValid() const;

    // Comparers

    struct Comparer_Id
      {
      const int m_command_id;
      Comparer_Id(int i_command_id);

      bool operator () (const ButtonInfo& i_command) const;
      };

    struct Comparer_StringId
      {
      const std::string m_string_id;
      Comparer_StringId(const std::string& m_string_id);

      bool operator () (const ButtonInfo& i_command) const;
      };

    };

  } // UI

#endif