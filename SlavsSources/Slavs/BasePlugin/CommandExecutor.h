#ifndef __COMMANDEXECUTOR_H__
#define __COMMANDEXECUTOR_H__

#include <SlavsServer/ICommandExecutor.h>

class MetaFactory;
class BaseObjectComposer;

namespace BasePlugin
	{


	class CommandExecutor : public SDK::GameCore::ICommandExecutor
		{
		private:
			MetaFactory&				m_meta_factory;
			BaseObjectComposer& m_base_composer;
			
		private:
			void Initialize();

		public:
			CommandExecutor(SDK::GameCore::CommandManager& i_manager, MetaFactory& i_meta_factory, BaseObjectComposer& i_composer);
			virtual ~CommandExecutor();

			virtual std::vector<std::string>															GetCommandsStrings(IController* ip_controller) const override;
			virtual std::vector<SDK::GameCore::CommandInformation>				GetCommands(IController* ip_controller) const override;

			virtual bool		CanExecuteImpl(IController* ip_controller, const SDK::GameCore::CommandInformation& i_command) const override;
			virtual void		ExecuteImpl(IController* ip_controller, const SDK::GameCore::CommandInformation& i_command, const boost::any& i_value) override;
		};

	} // BasePlugin

#endif