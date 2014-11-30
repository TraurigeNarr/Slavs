#ifndef __COMMANDINFO_H__
#define __COMMANDINFO_H__

namespace UI
  {

  struct CommandInfo
    {
    int m_id;
    int m_ui_id;
    int m_type_id;
    std::string m_string_id;

    CommandInfo();

    // Comparers

    struct Comparer_Id
      {
      const int m_command_id;
      Comparer_Id(int i_command_id);

      bool operator () (const CommandInfo& i_command) const;
      };

    struct Comparer_StringId
      {
      const std::string m_string_id;
      Comparer_StringId(const std::string& m_string_id);

      bool operator () (const CommandInfo& i_command) const;
      };
    };

  } // UI

#endif