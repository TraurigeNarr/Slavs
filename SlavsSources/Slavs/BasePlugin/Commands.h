#ifndef __COMMANDS_H__
#define __COMMANDS_H__

namespace BasePlugin
	{

	namespace Names
		{
		const std::string CATEGORY_PRODUCTION = "Production";
		const std::string CATEGORY_SOCIETY = "Society";

		const std::string COMMAND_CREATE_MANUFACTURE = "Create.Manufacture";
		const std::string COMMAND_CREATE_PRODUCTIONSTORE = "Create.Production.Store";
		const std::string COMMAND_CREATE_HIZHINA = "Create.Hizhina";
		}

	enum CommandCategories
		{
		Poduction,
		Society
		};

	enum Commands
		{
		CreateManufacture,
		CreateProductionStore,
		CreateHizhina
		};

	} // BasePlugin

#endif