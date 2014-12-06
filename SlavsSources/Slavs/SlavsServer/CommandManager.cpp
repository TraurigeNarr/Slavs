#include "CommandManager.h"
#include "ICommandExecutor.h"

#include "CommandInformation.h"

namespace SDK
	{

	namespace GameCore
		{

		CommandManager::CommandManager()
			: m_next_command_id(0)
			, m_next_category_id(0)
			{
			}

		CommandManager::~CommandManager()
			{
			}

		void CommandManager::RegisterCommandExecutor(ExecutorPtr ip_executor)
			{
			m_executors.push_back(std::move(ip_executor));

			const auto& executor_commands = m_executors.back()->GetCommands();
			for (const auto& command : executor_commands)
				command.m_global_id = m_next_command_id++;

			const auto& executor_categories = m_executors.back()->GetCategories();
			for (const auto& category : executor_categories)
				category.m_global_id = m_next_category_id++;
			}

		void CommandManager::UnregisterCommandExecutor(ICommandExecutor* ip_executor)
			{
			auto new_it = std::find_if(m_executors.begin(), m_executors.end(), [ip_executor](ExecutorPtr& p_executor)
				{
				return p_executor.get() == ip_executor;
				});

			m_executors.erase(new_it, m_executors.end());
			}

		std::vector<std::string> CommandManager::GetCommandStrings(IController* ip_controller) const
			{
			std::vector<std::string> controller_commands;
			for (const auto& p_executor : m_executors)
				{
				auto commands = p_executor->GetCommands(ip_controller);
				controller_commands.reserve(controller_commands.size() + commands.size());
				for (auto& command : commands)
					controller_commands.push_back(command.CommandString());
				}

			return controller_commands;
			}

		std::vector<std::string> CommandManager::GetCommandCategories() const
			{
			std::vector<std::string> categories;

			for (const auto& p_executor : m_executors)
				{
				auto executor_categories = p_executor->GetCommandCategories();
				categories.reserve(categories.size() + executor_categories.size());
				std::copy(executor_categories.begin(), executor_categories.end(), std::inserter(categories, categories.end()));
				}

			return categories;
			}

		ICommandExecutor* CommandManager::CanExecute(IController* ip_controller, int i_command_id) const
			{
			for (const auto& p_executor : m_executors)
				if (p_executor->CanExecute(ip_controller, i_command_id))
					return p_executor.get();
			return nullptr;
			}

		void CommandManager::Execute(IController* ip_controller, int i_command_id, const boost::any& i_data)
			{
			if (auto p_executor = CanExecute(ip_controller, i_command_id))
				Execute(ip_controller, p_executor, i_command_id, i_data);
			else
				throw std::logic_error("<CommandManager> No executor for command");
			}

		void CommandManager::Execute(IController* ip_controller, ICommandExecutor* ip_executor, int i_command_id, const boost::any& i_data)
			{
			if (ip_executor == nullptr)
				throw std::logic_error("<CommandManager> Null executor");
			ip_executor->Execute(ip_controller, i_command_id, i_data);
			}

		} // GameCore

	} // SDK