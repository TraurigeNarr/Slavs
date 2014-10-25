#ifndef ObjectInformation_h
#define ObjectInformation_h
//client
#include "ClientEnums.h"
//common
#include <Game/Enumerations.h>
#include "Game/GameResources.h"
#include <Utilities/StringUtilities.h>
//standard
#include <map>
#include <string>
#include <vector>

enum SField
{
	SF_Action,
	SF_Experience,
	SF_HP,
	SF_Energy,
	SF_BattleSpirit,
	SF_Attack,
	SF_Armor,
	SF_Resource,
};

struct ObjectInformation
{
protected:
	std::vector<ButtonID>			vCommands;
	std::string						sName;
public:
	std::vector<std::string>		vInformation;
	bool							bStore;
	int   						oType;

									ObjectInformation(const std::string& name) : sName(name)
									{ Clear(); bStore = false; }

	void							Clear()
									{vInformation.clear(); vCommands.clear();}

	template<class T>
	void							AddInformation(T val)
									{vInformation.push_back(StringUtilities::t_to_string(val));}

	void							AddCommand(ButtonID btnID)
									{
										if(Btn_None != btnID)
											vCommands.push_back(btnID);
									}
	const std::vector<ButtonID>&	GetCommands() const { return vCommands; }
	const std::string&				GetName() const { return sName; }
};

struct ResourceInformation
{
	GameResourceGroup	grGroup;
	GameResourceType	grType;
	std::string			sValue;
};

#endif