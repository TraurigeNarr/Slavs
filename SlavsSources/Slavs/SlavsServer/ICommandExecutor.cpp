#include "ICommandExecutor.h"

#include <Common/Utilities/StringUtilities.h>

#include <cassert>

namespace SDK
	{

	namespace GameCore
		{

		std::vector<std::string>	ICommandExecutor::GetCommandCategories() const
			{
			std::vector<std::string> categories;

			categories.reserve(m_categories.size());
			for (const CommandCategory& category : m_categories)
				categories.push_back(category.CategoryString());

			return categories;
			}

		bool ICommandExecutor::CanExecute(IController* ip_controller, int i_command_id) const
			{
			auto it = std::find_if(m_commands.begin(), m_commands.end(), CommandInformation::Comparer_GlobalId(i_command_id));
			if (it == m_commands.end())
				return false;
			return CanExecuteImpl(ip_controller, *it);
			}

		void ICommandExecutor::Execute(IController* ip_controller, int i_command_id, const boost::any& i_value)
			{
			auto it = std::find_if(m_commands.begin(), m_commands.end(), CommandInformation::Comparer_GlobalId(i_command_id));
			if (it == m_commands.end())
				{
				assert(false);
				throw std::logic_error("No command in executor");
				}

			ExecuteImpl(ip_controller, *it, i_value);
			}

		} // GameCore

	} // SDK