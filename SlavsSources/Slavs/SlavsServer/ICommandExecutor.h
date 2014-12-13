#ifndef __ICOMMANDEXECUTOR_H__
#define __ICOMMANDEXECUTOR_H__

#include "SlavsServerAPI.h"

#include "CommandInformation.h"

#include <boost/any.hpp>
#include <vector>

class IController;

namespace SDK
	{

	namespace GameCore
		{

		class CommandManager;

		typedef std::pair<int, std::string> CategoryPair;

		class ICommandExecutor
			{
			protected:
				CommandManager&									m_manager;

				std::vector<CommandInformation> m_commands;
				std::vector<CommandCategory>		m_categories;

			private:
				virtual bool	CanExecuteImpl(IController* ip_controller, const CommandInformation& i_command) const = 0;
				virtual void	ExecuteImpl(IController* ip_controller, const CommandInformation& i_command, const boost::any& i_value) = 0;

			public:
				ICommandExecutor(CommandManager& i_manager)
					: m_manager(i_manager)
					{}
				virtual ~ICommandExecutor(){}

				SLAVS_SERVER_EXPORT std::vector<std::string>		GetCommandCategories() const;
				const std::vector<CommandInformation>&					GetCommands() const;
				const std::vector<CommandCategory>&							GetCategories() const;
				SLAVS_SERVER_EXPORT bool												GetExecutor(IController* ip_controller, int i_command_id) const;
				SLAVS_SERVER_EXPORT void												Execute(IController* ip_controller, int i_command_id, const boost::any& i_value);

				virtual std::vector<std::string>								GetCommandsStrings(IController* ip_controller) const = 0;
				virtual std::vector<CommandInformation>					GetCommands(IController* ip_controller) const = 0;
				
			};

		inline const std::vector<CommandInformation>&	ICommandExecutor::GetCommands() const
			{
			return m_commands;
			}

		inline const std::vector<CommandCategory>&	ICommandExecutor::GetCategories() const
			{
			return m_categories;
			}

		} // GameCore

	} // SDK

#endif