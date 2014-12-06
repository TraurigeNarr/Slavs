#ifndef __COMMANDINFORMATION_H__
#define __COMMANDINFORMATION_H__

#include "SlavsServerAPI.h"

#include <string>

namespace SDK
	{

	namespace GameCore
		{
		// CommandInformation <int, IController, modifiers, additional data?>
		struct CommandInformation
			{
			int					m_id;
			int					m_category_id;
			mutable int m_global_id;
			std::string m_string_id;

			SLAVS_SERVER_EXPORT CommandInformation();
			SLAVS_SERVER_EXPORT CommandInformation(int i_id, int i_category_id, const std::string& i_string_id);

			// Format: <CommandType>;<CommandStringId>;<GlobalCommandID>
			SLAVS_SERVER_EXPORT std::string CommandString() const;

			// Comparers

			struct Comparer_Id
				{
				const int m_command_id;
				SLAVS_SERVER_EXPORT Comparer_Id(int i_command_id);

				SLAVS_SERVER_EXPORT bool operator () (const CommandInformation& i_command) const;
				};

			struct Comparer_GlobalId
				{
				const int m_command_id;
				SLAVS_SERVER_EXPORT Comparer_GlobalId(int i_command_id);

				SLAVS_SERVER_EXPORT bool operator () (const CommandInformation& i_command) const;
				};

			struct Comparer_StringId
				{
				const std::string m_string_id;
				SLAVS_SERVER_EXPORT Comparer_StringId(const std::string& m_string_id);

				SLAVS_SERVER_EXPORT bool operator () (const CommandInformation& i_command) const;
				};
			};

		struct CommandCategory
			{
			int					m_category_id;
			mutable int m_global_id;
			std::string m_string_id;

			SLAVS_SERVER_EXPORT CommandCategory();
			SLAVS_SERVER_EXPORT CommandCategory(int i_category_id, const std::string& i_string_id);

			// Format: <CommandTypeSting>;<CommandTypeID>
			SLAVS_SERVER_EXPORT std::string CategoryString() const;
			};

		} // GameCore

	} // SDK

#endif