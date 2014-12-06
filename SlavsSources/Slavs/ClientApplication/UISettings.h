#ifndef __UISETTINGS_H__
#define __UISETTINGS_H__

#include "ButtonInfo.h"
#include "CommandInfo.h"

class TiXmlElement;

namespace UI
  {

  typedef std::vector<ButtonInfo> ButtonsInformation;
  typedef std::vector<CommandInfo> CommandsInfrmation;

  class UISettings
    {
    private:
      typedef std::pair<int/*command_type*/, int/*ui_id*/> CommandTypeInformation;
    private:
			CommandsInfrmation m_server_commands;
			ButtonsInformation m_commands;
      std::vector<CommandTypeInformation> m_command_types;

      size_t m_next_command_id;

    private:
      void ParseButtons(const TiXmlElement* ip_buttons_root);
			void ParseSchemes(const TiXmlElement* ip_schemes_root);
			void ParseImageSets(const TiXmlElement* ip_imagesets_root);

      // adds undefined command
      ButtonInfo& AddUndefinedCommand(int i_id, const std::string& i_string_id);

    public:
      UISettings();
      ~UISettings();

      void LoadFromFile(const std::string& i_file_path);

      void ClearCommands();
      // Parse special format of string and extracted from it information about type
      //    as int and name as string
      // Format: <CommandTypeSting>;<CommandTypeID>
      //  SocialBuilding;1
			void AddCommandCategoryFromString(const std::string& i_string);
      // Format: <CommandType>;<CommandStringId>;<CommandID>;<DisplayName>;<Tooltip>
      //  1;Build.Store;12;Store;Build store for getting place for goods
      //    If there is no DisplayName -> gets same as CommandStringId with spaces instead '.'
      //    If there is no tooltip -> same as DisplayName
      void AddCommandFromString(const std::string& i_string);

      const ButtonInfo& GetButtonInfo(int i_ui_id) const;
      const ButtonInfo& GetButtonInfoFromType(int i_type_id) const;

			const CommandsInfrmation& GetServerCommands() const;
    };

	inline const CommandsInfrmation& UISettings::GetServerCommands() const
    {
    return m_server_commands;
    }

  } // UI

#endif