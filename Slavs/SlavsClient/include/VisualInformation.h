#ifndef VisualInformation_h
#define VisualInformation_h

#include "ButtonInformation.h"
#include "ClientEnums.h"

#include <Patterns/Singleton.h>

#include <map>
#include <vector>

class CommandData;

typedef std::map<ButtonID, ButtonInformation*> ButtonsMap;
typedef std::map<ButtonID, std::vector<ButtonID>> BuildingsOnClick;
typedef std::map<ButtonID, CommandData*> BtnToCmdMap;

class VisualInformation
{
public:
	VisualInformation();
	~VisualInformation();

	std::vector<ButtonID>			GetBuildingGroups() const;
	bool							IsBuildingGroupBtn(ButtonID id) const;

	const std::vector<ButtonID>&	GetBuildings(ButtonID groupBtn) const;
	ButtonInformation*				GetBtnInformation(ButtonID id) const;
	CommandData*					GetCommandData(ButtonID id) const;

	static ButtonID					DefineBtnID(const std::string& name);
	static BuildingGroup			DefineBuildingGroup(const std::string& name);
protected:
	void							Initialize();
	static CommandData*				GetLinkedCD(ButtonID btnID);

	ButtonsMap						m_mButtonInformation;
	BuildingsOnClick				m_mBuildingsOnClick;
	BtnToCmdMap						m_mBtnToCmdMap;
};

#endif