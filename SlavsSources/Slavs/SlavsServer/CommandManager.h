#ifndef	__COMMANDMANAGER_H__
#define __COMMANDMANAGER_H__

#include "SlavsServerAPI.h"

#include <memory>
#include <vector>

#include <boost/any.hpp>

class IController;

namespace SDK
	{

	namespace GameCore
		{
		class ICommandExecutor;

		class CommandManager
			{
			private:
				typedef std::unique_ptr<ICommandExecutor> ExecutorPtr;
				std::vector<ExecutorPtr>	m_executors;
				// use for global identification of commands - plugins should use local id
				//		transformation will be inside ICommandExecutor
				size_t										m_next_command_id;
				size_t										m_next_category_id;

			public:
				SLAVS_SERVER_EXPORT CommandManager();
				SLAVS_SERVER_EXPORT ~CommandManager();

				SLAVS_SERVER_EXPORT void RegisterCommandExecutor(ExecutorPtr ip_executor);
				SLAVS_SERVER_EXPORT void UnregisterCommandExecutor(ICommandExecutor* ip_executor);

				SLAVS_SERVER_EXPORT std::vector<std::string> GetCommandStrings(IController* ip_controller) const;
				SLAVS_SERVER_EXPORT std::vector<std::string> GetCommandCategories() const;

				// typical usage
				// if (auto p_executor = CanExecute(id))
				//		Execute(p_executor, id, data);
				SLAVS_SERVER_EXPORT ICommandExecutor* CanExecute(IController* ip_controller, int i_command_id) const;
				// throws exception if there is no executor which can handle this command
				SLAVS_SERVER_EXPORT void Execute(IController* ip_controller, int i_command_id, const boost::any& i_data);
				// thriw exception if ip_executor = nullptr 
				SLAVS_SERVER_EXPORT void Execute(IController* ip_controller, ICommandExecutor* ip_executor, int i_command_id, const boost::any& i_data);
			};

		} // GameCore

	} // SDK

#endif