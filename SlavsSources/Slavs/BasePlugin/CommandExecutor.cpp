#include "stdafx.h"

#include "CommandExecutor.h"
#include "Commands.h"

#include "BaseObjectComposer.h"
#include "TypeEnumerations.h"

#include <SlavsServer/CommandManager.h>
#include <SlavsServer/include/IController.h>
#include <SlavsServer/Game/GameContext.h>
#include <PluginSystem/MetaFactory.h>

#include <Common/Utilities/StringUtilities.h>

#include <boost/any.hpp>

using namespace SDK;

enum Commands
	{
	CreateManufacture = 0,
	CreateProductionStore = 1,
	CreateHouse = 2
	};

namespace BasePlugin
	{

	CommandExecutor::CommandExecutor(SDK::GameCore::CommandManager& i_manager, MetaFactory& i_meta_factory, BaseObjectComposer& i_composer)
		: ICommandExecutor(i_manager)
		, m_meta_factory(i_meta_factory)
		, m_base_composer(i_composer)
		{
		Initialize();
		}

	CommandExecutor::~CommandExecutor()
		{		}

	void CommandExecutor::Initialize()
		{
		// Production
		m_categories.push_back(GameCore::CommandCategory(CommandCategories::Poduction, Names::CATEGORY_PRODUCTION));
		m_commands.push_back(GameCore::CommandInformation(Commands::CreateProductionStore, 0, Names::COMMAND_CREATE_PRODUCTIONSTORE));
		m_commands.push_back(GameCore::CommandInformation(Commands::CreateManufacture, 0, Names::COMMAND_CREATE_MANUFACTURE));

		// Society
		m_categories.push_back(GameCore::CommandCategory(CommandCategories::Society, Names::CATEGORY_SOCIETY));
		m_commands.push_back(GameCore::CommandInformation(Commands::CreateHizhina, 1, Names::COMMAND_CREATE_HIZHINA));
		}

	std::vector<SDK::GameCore::CommandInformation> CommandExecutor::GetCommands(IController* ip_controller) const
		{
		return m_commands;
		}

	std::vector<std::string> CommandExecutor::GetCommandsStrings(IController* ip_controller) const
		{
		std::vector<std::string> commands;
		
		commands.reserve(m_commands.size());
		for (const auto& command : m_commands)
			commands.push_back(command.CommandString());

		return commands;
		}

	bool CommandExecutor::CanExecuteImpl(IController* ip_controller, const GameCore::CommandInformation& i_command) const
		{
		// check condition for controller -> now every command can be executed
		return true;
		}

	void CommandExecutor::ExecuteImpl(IController* ip_controller, const GameCore::CommandInformation& i_command, const boost::any& i_value)
		{
		int global_id = -1;
		// parse local id of command
		switch (i_command.m_id)
			{
			case Commands::CreateProductionStore:
				global_id = m_base_composer.GetObjectGlobalID(BasePlugin::ObjectType::OT_PRODUCTION_STORE);
				break;
			case Commands::CreateManufacture:
				global_id = m_base_composer.GetObjectGlobalID(BasePlugin::ObjectType::OT_MANUFACTURE);
				break;
			case Commands::CreateHizhina:
				global_id = m_base_composer.GetObjectGlobalID(BasePlugin::ObjectType::OT_HIZHINA);
				break;
			}

		if (global_id != -1)
			{
			Vector2D position = boost::any_cast<Vector2D>(i_value);
			ip_controller->GetGameContext().AddObject(global_id, position, ip_controller->GetMask());
			}
		}

	} // BasePlugin