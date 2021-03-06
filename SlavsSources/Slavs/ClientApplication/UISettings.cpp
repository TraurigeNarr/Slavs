#include "stdafx.h"

#include "UISettings.h"

#include <Common/Utilities/StringUtilities.h>
#include <Common/Utilities/XmlUtilities.h>

#include <boost/lexical_cast.hpp>

namespace UI
  {

	extern void RegisterDialogs(UISettings& o_settings);

  const std::string DEFAULT_BUTTON_TYPE = "SlavsLook/Button";

  UISettings::UISettings()
    : m_next_command_id(0)
    {
		RegisterDialogs(*this);
    }

  UISettings::~UISettings()
    {

    }

  void UISettings::ParseButtons(const TiXmlElement* ip_buttons_root)
    {
    m_commands.clear();
    m_next_command_id++;

    const TiXmlElement* p_child = nullptr;
    const TiXmlElement* p_undefined_object = nullptr;
    while (p_child = XmlUtilities::IterateChildElements(ip_buttons_root, p_child))
      {
      std::string button_string_id = p_child->Value();

      m_commands.push_back(ButtonInfo(button_string_id));
      ButtonInfo* p_info = &m_commands.back();
      p_info->m_ui_id = m_next_command_id++;
      // TODO create method for extracting values from node
      //    <Node>Value</Node>
      p_info->m_display_name = p_child->FirstChild("DisplayName")->FirstChild()->Value();
      p_info->m_tooltip = p_child->FirstChild("Tooltip")->FirstChild()->Value();

			if (auto p_display_text = p_child->FirstChild("DisplayText"))
				{
				std::string str = p_display_text->FirstChild()->Value();
				p_info->m_display_text = str.compare("true") == 0;
				}
			else
				p_info->m_display_text = false;

      if (auto p_button_type = p_child->FirstChild("ButtonType"))
        p_info->m_button_type = p_button_type->FirstChild()->Value();
      else
        p_info->m_button_type = DEFAULT_BUTTON_TYPE;
      }
    }

	void UISettings::ParseSchemes(const TiXmlElement* ip_schemes_root)
		{
		const TiXmlElement* p_child = nullptr;
		const TiXmlElement* p_undefined_object = nullptr;
		while (p_child = XmlUtilities::IterateChildElements(ip_schemes_root, p_child))
			{
			auto path = XmlUtilities::GetStringAttribute(p_child, "path", "");
			try
				{
				CEGUI::SchemeManager::getSingleton().createFromFile(path);
				}
			catch (std::exception& e)
				{
				// TODO log
				}

			}
		}

	void UISettings::ParseImageSets(const TiXmlElement* ip_imagesets_root)
		{
		const TiXmlElement* p_child = nullptr;
		const TiXmlElement* p_undefined_object = nullptr;
		while (p_child = XmlUtilities::IterateChildElements(ip_imagesets_root, p_child))
			{
			auto path = XmlUtilities::GetStringAttribute(p_child, "path", "");
			try
				{
				CEGUI::ImageManager::getSingleton().loadImageset(path);
				}
			catch (Ogre::FileNotFoundException& e)
				{
				// TODO log
				}
			
			}
		}

	void UISettings::ParseInformation(const TiXmlElement* ip_imagesets_root)
		{
		const TiXmlElement* p_child = nullptr;
		const TiXmlElement* p_undefined_object = nullptr;
		while (p_child = XmlUtilities::IterateChildElements(ip_imagesets_root, p_child))
			{
			auto string_id = p_child->Value();
			auto type = XmlUtilities::GetStringAttribute(p_child, "type", "");
			if (type == "information")
				{
				auto caption = p_child->FirstChild("Caption")->FirstChild()->Value();
				auto text = p_child->FirstChild("Text")->FirstChild()->Value();
				auto image = p_child->FirstChild("Image")->FirstChild()->Value();
				auto id = boost::lexical_cast<int>(p_child->FirstChild("Id")->FirstChild()->Value());

				m_information_views.push_back(InformationPtr(new Dialog(WindowType::Information, id, string_id, text, caption, image)));
				}
			else if (type == "clip")
				{
				ParseClip(p_child);
				}
			}
		}

	void UISettings::ParseClip(const TiXmlElement* ip_clip_root)
		{
		auto string_id = ip_clip_root->Value();
		auto caption = ip_clip_root->FirstChild("Caption")->FirstChild()->Value();
		auto id = boost::lexical_cast<int>(ip_clip_root->FirstChild("Id")->FirstChild()->Value());
		Clip clip(id, string_id, caption);

		const TiXmlElement* p_clip = ip_clip_root->FirstChildElement("Clip");
		const TiXmlElement* p_child = nullptr;
		while (p_child = XmlUtilities::IterateChildElements(p_clip, p_child))
			{
			float show_time = XmlUtilities::GetRealAttribute(p_child, "show_time", 0.5f);
			auto text = p_child->FirstChild("Text")->FirstChild()->Value();
			auto image = p_child->FirstChild("Image")->FirstChild()->Value();
			clip.AddFrame(text, image, show_time);
			}
		m_information_views.push_back(InformationPtr(new Clip(clip)));
		}

  void UISettings::LoadFromFile(const std::string& i_file_path)
    {
    TiXmlDocument document;

    if (!XmlUtilities::LoadXmlDocument(i_file_path, document))
      throw std::invalid_argument("Can not load file");

		const TiXmlElement* p_root = document.RootElement();

    const TiXmlElement* p_root_buttons = p_root->FirstChildElement("Buttons");
    if (p_root_buttons)
      ParseButtons(p_root_buttons);

		const TiXmlElement* p_root_schemes = p_root->FirstChildElement("Schemes");
		if (p_root_schemes)
			ParseSchemes(p_root_schemes);

		const TiXmlElement* p_root_imagesets = p_root->FirstChildElement("ImageSets");
		if (p_root_imagesets)
			ParseImageSets(p_root_imagesets);

		const TiXmlElement* p_root_information = p_root->FirstChildElement("Information");
		if (p_root_information)
			ParseInformation(p_root_information);
    }

  void UISettings::ClearCommands()
    {
    m_commands.clear();
    }

  ButtonInfo& UISettings::AddUndefinedCommand(int i_id, const std::string& i_string_id)
    {
    auto button_it = std::find_if(m_commands.begin(), m_commands.end(), ButtonInfo::Comparer_StringId("Undefined"));
    if (button_it != m_commands.end())
      {
      m_commands.push_back(*button_it);
      m_commands.back().m_string_id = i_string_id;
      }
    else
      {
      m_commands.push_back(ButtonInfo(i_string_id));
      m_commands.back().m_button_type = DEFAULT_BUTTON_TYPE;
      }
    m_commands.back().m_ui_id = m_next_command_id++;
    m_commands.back().m_display_name = i_string_id;
    m_commands.back().m_tooltip = i_string_id;
    
    return m_commands.back();
    }

  void UISettings::AddCommandCategoryFromString(const std::string& i_string)
    {
    auto tokens = StringUtilities::Tokenize(i_string, ";:()");
    if (tokens.size() != 2)
      throw std::logic_error("Tokens size should be 2");

    std::string command_type = tokens[0];
    int command_id = boost::lexical_cast<int>(tokens[1]);

    int ui_id = -1;

    auto button_it = std::find_if(m_commands.begin(), m_commands.end(), ButtonInfo::Comparer_StringId(command_type));
    if (button_it != m_commands.end())
      ui_id = button_it->m_ui_id;
    else
      ui_id = AddUndefinedCommand(command_id, command_type).m_ui_id;

    m_command_types.push_back(std::make_pair(command_id, ui_id));
    }

  void UISettings::AddCommandFromString(const std::string& i_string)
    {
    auto tokens = StringUtilities::Tokenize(i_string, ";:()");
    if (tokens.size() < 3)
      throw std::logic_error("Tokens size should be more or equal 3");
    int command_type = boost::lexical_cast<int>(tokens[0]);
    std::string command_string_id = tokens[1];
    int command_id = boost::lexical_cast<int>(tokens[2]);

    // we find button in configs that we loaded
    auto button_it = std::find_if(m_commands.begin(), m_commands.end(), ButtonInfo::Comparer_StringId(command_string_id));
    ButtonInfo* p_button_info = nullptr;
    if (button_it != m_commands.end())
      p_button_info = &(*button_it);
    else
      p_button_info = &AddUndefinedCommand(command_id, command_string_id);

    if (tokens.size() > 3)
      p_button_info->m_display_name = tokens[3];
    if (tokens.size() > 4)
      p_button_info->m_tooltip = tokens[4];
    int ui_id = p_button_info->m_ui_id;

    m_server_commands.push_back(CommandInfo());
    m_server_commands.back().m_id = command_id;
    m_server_commands.back().m_string_id = command_string_id;
    m_server_commands.back().m_type_id = command_type;
    m_server_commands.back().m_ui_id = ui_id;
    }

  const ButtonInfo& UISettings::GetButtonInfo(int i_ui_id) const
    {
    auto button_it = std::find_if(m_commands.begin(), m_commands.end(), ButtonInfo::Comparer_Id(i_ui_id));
    if (button_it != m_commands.end())
      {
      return *button_it;
      }
    else
      throw std::logic_error("No such ui id");
    }

  const ButtonInfo& UISettings::GetButtonInfoFromType(int i_type_id) const
    {
    auto type_it = std::find_if(m_command_types.begin(), m_command_types.end(), [i_type_id](const std::pair<int, int>& type_pair)
      {
      return type_pair.first == i_type_id;
      });
    if (type_it == m_command_types.end())
      throw std::logic_error("No such command type");

    return GetButtonInfo(type_it->second);
    }

	const Dialog& UISettings::GetDialog(int i_id) const
		{
		auto dlg_it = std::find_if(m_information_views.begin(), m_information_views.end(), [i_id](const InformationPtr& dlg)
			{
			return dlg->GetInformation().m_id == i_id;
			});

		if (dlg_it == m_information_views.end() || typeid(*dlg_it) != typeid(Dialog))
			throw std::logic_error("No such dialog");
		return static_cast<const Dialog&>(**dlg_it);
		}

	const Clip& UISettings::GetClip(int i_id) const
		{
		auto dlg_it = std::find_if(m_information_views.begin(), m_information_views.end(), [i_id](const InformationPtr& dlg)
			{
			return dlg->GetInformation().m_id == i_id;
			});

		if (dlg_it == m_information_views.end() || typeid(*dlg_it) != typeid(Clip))
			throw std::logic_error("No such clip");

		return static_cast<const Clip&>(**dlg_it);
		}

	const InformationView& UISettings::GetInformation(int i_id) const
		{
		auto dlg_it = std::find_if(m_information_views.begin(), m_information_views.end(), [i_id](const InformationPtr& dlg)
			{
			return dlg->GetInformation().m_id == i_id;
			});

		if (dlg_it == m_information_views.end())
			throw std::logic_error("No such information");
		return **dlg_it;
		}

  } // UI