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

    ButtonInfo(int i_ui_id,
              const std::string& i_string_id,
              const std::string& i_name,
              const std::string& i_button_type,
              const std::string& i_tooltip = std::string());
    
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