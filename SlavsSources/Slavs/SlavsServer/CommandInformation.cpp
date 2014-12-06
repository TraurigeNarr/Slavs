#include "CommandInformation.h"

#include <Common/Utilities/StringUtilities.h>

namespace SDK
	{

	namespace GameCore
		{
		
		CommandInformation::CommandInformation()
			: m_id(-1)
			, m_category_id(-1)
			, m_string_id()
			{			}

		CommandInformation::CommandInformation(int i_id, int i_category_id, const std::string& i_string_id)
			: m_id(i_id)
			, m_category_id(i_category_id)
			, m_string_id(i_string_id)
			{			}

		std::string CommandInformation::CommandString() const
			{
			std::string command_string;
			command_string += StringUtilities::t_to_string(m_category_id);
			command_string += ";";
			command_string += m_string_id;
			command_string += ";";
			command_string += StringUtilities::t_to_string(m_global_id);
			return command_string;
			}

		///////////////////////////////////////////////////////////////////////////
		// Comparers
		///////////////////////////////////////////////////////////////////////////
		// ID

		CommandInformation::Comparer_Id::Comparer_Id(int i_command_id)
			: m_command_id(i_command_id)
			{}

		bool CommandInformation::Comparer_Id::operator() (const CommandInformation& i_command) const
			{
			return i_command.m_id == m_command_id;
			}

		///////////////////////////////////////////////////////////////////////////
		// Global id

		CommandInformation::Comparer_GlobalId::Comparer_GlobalId(int i_command_id)
			: m_command_id(i_command_id)
			{}

		bool CommandInformation::Comparer_GlobalId::operator() (const CommandInformation& i_command) const
			{
			return i_command.m_global_id == m_command_id;
			}

		///////////////////////////////////////////////////////////////////////////
		// String id
		CommandInformation::Comparer_StringId::Comparer_StringId(const std::string& i_string_id)
			: m_string_id(i_string_id)
			{}

		bool CommandInformation::Comparer_StringId::operator () (const CommandInformation& i_command) const
			{
			return i_command.m_string_id == m_string_id;
			}

		///////////////////////////////////////////////////////////////////////////
		// CommandCategory

		CommandCategory::CommandCategory()
			: m_category_id(-1)
			, m_global_id(-1)
			, m_string_id()
			{			}

		CommandCategory::CommandCategory(int i_category_id, const std::string& i_string_id)
			: m_category_id(i_category_id)
			, m_global_id(-1)
			, m_string_id(i_string_id)
			{			}

		std::string CommandCategory::CategoryString() const
			{
			std::string command_string;
			command_string += m_string_id;
			command_string += ";";
			command_string += StringUtilities::t_to_string(m_category_id);
			return command_string;
			}

		} // GameCore

	} // SDK