#include "VisualInformation.h"
//client
#include "ButtonInformation.h"
//common
#include <Game/CommandData.h>
#include <Utilities/FileUtilities.h>
#include <Utilities/TemplateFunctions.h>
#include <Utilities/XmlUtilities.h>
//standard
#include <algorithm>
#include <string>
//define some constants
const std::string VisualInfoFileName = "client\\configs\\VisualInfo.xml";
const std::string BuildingsBoxes = "buildingsBoxes";
const std::string  Buttons = "buttons";
const std::string  Actors = "actors";

VisualInformation::VisualInformation()
{
	Initialize();
}

VisualInformation::~VisualInformation()
{
	ClearSTLMap(m_mButtonInformation);
	ClearSTLMap(m_mBtnToCmdMap);
	/*std::for_each(m_mBuildingsOnClick.begin(), m_mBuildingsOnClick.end(), [this](std::pair<BuildingGroup, std::vector<ButtonID>> vec)
	{
		vec.second.clear();
	});*/
}


std::vector<ButtonID> VisualInformation::GetBuildingGroups() const
{

	std::vector<ButtonID> vecBtn;

	std::for_each(m_mBuildingsOnClick.begin(), m_mBuildingsOnClick.end(), [&vecBtn](std::pair<ButtonID, std::vector<ButtonID>> p)
	{
		vecBtn.push_back(p.first);
	});

	return vecBtn;
}

bool VisualInformation::IsBuildingGroupBtn(ButtonID id) const
{
	BuildingsOnClick::const_iterator iter = m_mBuildingsOnClick.find(id);

	return m_mBuildingsOnClick.end() != iter;
}

const std::vector<ButtonID>& VisualInformation::GetBuildings(ButtonID groupBtn) const
{
	BuildingsOnClick::const_iterator iter = m_mBuildingsOnClick.find(groupBtn);
	
	if(m_mBuildingsOnClick.end() != iter)
		return iter->second;
	return *(new std::vector<ButtonID>());
}

ButtonInformation* VisualInformation::GetBtnInformation(ButtonID id) const
{
	ButtonsMap::const_iterator iter = m_mButtonInformation.find(id);

	if(m_mButtonInformation.end() != iter)
		return iter->second;
	return NULL;
}

CommandData* VisualInformation::GetCommandData(ButtonID id) const
{
	BtnToCmdMap::const_iterator iter = m_mBtnToCmdMap.find(id);

	if(m_mBtnToCmdMap.end() != iter)
		return iter->second;
	
	return NULL;
}

void VisualInformation::Initialize()
{
	ClearSTLMap(m_mButtonInformation);
	ClearSTLMap(m_mBtnToCmdMap);
	TiXmlDocument document;
	XmlUtilities::LoadXmlDocument(FileUtilities::JoinPath(FileUtilities::GetApplicationDirectory(), VisualInfoFileName), document);

	TiXmlElement *rootElem = document.RootElement();

	std::string elementName = "";

	const TiXmlElement* childElement = 0;
	std::string btnElementName = "";
	const TiXmlElement* btnChildElement = 0;
	ButtonID buttonID = Btn_None;

	while ((childElement = XmlUtilities::IterateChildElements(rootElem, childElement)))
	{
		elementName = childElement->Value();
		//----------------------------------------------------------------------
		if(Buttons == elementName || Actors == elementName)
		{
			CommandData* cData = NULL;
			while ((btnChildElement = XmlUtilities::IterateChildElements(childElement, btnChildElement)))
			{
				btnElementName = btnChildElement->Value();
				buttonID = DefineBtnID(btnElementName);
				if(Btn_None != buttonID)
				{
					//link command data and button id
					cData = VisualInformation::GetLinkedCD(buttonID);
					if(NULL != cData)
						m_mBtnToCmdMap.insert(std::pair<ButtonID, CommandData*>(buttonID, cData));
					//get specific attributes for button
					int pos						        = XmlUtilities::GetIntAttribute(btnChildElement, "position", 0);
					std::string text			    = XmlUtilities::GetStringAttribute(btnChildElement, "text", "");
					std::string toolTip			  = XmlUtilities::GetStringAttribute(btnChildElement, "tooltip", "");
					ButtonInformation *bInfo	= new ButtonInformation(buttonID, pos, text, toolTip);
					m_mButtonInformation[buttonID] = bInfo;
				}
			}
		}
		//-----------------------------------------------------------------------
		else
		{
			BuildingGroup bGroup	= DefineBuildingGroup(XmlUtilities::GetStringAttribute(childElement, "type", ""));
			ButtonID bGroupBtn		= DefineBtnID(XmlUtilities::GetStringAttribute(childElement, "button"));
			if(BType_Undefined != bGroup && Btn_None != bGroupBtn)
			{
				while ((btnChildElement = XmlUtilities::IterateChildElements(childElement, btnChildElement)))
				{
					buttonID = DefineBtnID(btnChildElement->Value());
					if(buttonID != Btn_None)
						m_mBuildingsOnClick[bGroupBtn].push_back(buttonID);
				}
				buttonID = Btn_None;
			}
		}
		//-----------------------------------------------------------------------
	}
}

ButtonID VisualInformation::DefineBtnID(const std::string& name)
{
#pragma region Common
	if(MoveName == name)
	{
		return Btn_Move;
	}
	if(StopName == name)
	{
		return Btn_Stop;
	}
	if(AttackName == name)
	{
		return Btn_Attack;
	}
	if(HoldName == name)
	{
		return Btn_Hold;
	}
#pragma endregion

	if(CreateDetinets == name)
		return Btn_Create_Detinets;
	if(CreateHizhina == name)
		return Btn_Create_Hizhina;
	if(CreateCasern == name)
		return Btn_Create_Casern;
	if("create_sawmill" == name)
		return Btn_Create_Sawmill;
	if("create_stoneQuarry" == name)
		return Btn_Create_StoneQuarry;
	if("create_wheatField" == name)
		return Btn_Create_WheatField;
	if("create_vegetableGarden" == name)
		return Btn_Create_VegetableGarden;
	if("create_garden" == name)
		return Btn_Create_Garden;
	if("create_farm" == name)
		return Btn_Create_Farm;
	if("create_mill" == name)
		return Btn_Create_Mill;
	if("create_tannery" == name)//кожевня
		return Btn_Create_Tannery;
	if("create_bakery" == name)
		return Btn_Create_Bakery;
	if("create_treasury" == name)
		return Btn_Create_Treasury;
	if("create_rationStore" == name)
		return Btn_Create_RationStore;
	if("create_storeHouse" == name)
		return Btn_Create_StoreHouse;
	
	if(CreateHovel == name)
		return Btn_Create_Hovel;
	if(CreateHozDvor == name)
		return Btn_Create_HozDvor;
	if(CreateIzba == name)
		return Btn_Create_Izba;
	if(CreateZemlyanka == name)
		return Btn_Create_Zemlyanka;

#pragma region Create Buildings Dark Obedient
	if(CreateHOTD == name)
	{
		return Btn_Create_HOTD;
	}
	if(CreateHOD == name)
	{
		return Btn_Create_HOD;
	}
	if(CreateKurgan == name)
	{
		return Btn_Create_HOTD;
	}
	if(CreateLachuga == name)
	{
		return Btn_Create_Lachuga;
	}
	if(CreateNora == name)
	{
		return Btn_Create_Nora;
	}
	if(CreatePrison == name)
	{
		return Btn_Create_Prison;
	}
	if(CreateSklep == name)
	{
		return Btn_Create_Sklep;
	}
#pragma endregion

#pragma region Building Groups
	if(BGroup_general == name)
		return Btn_BGroup_General;
	if(BGroup_military == name)
		return Btn_BGroup_Military;
	if("bType_resources" == name)
		return Btn_BGroup_Resources;
	if("bType_processingRes" == name)
		return Btn_BGroup_ProcessingResources;
#pragma endregion

	if("rg_material" == name)
		return Btn_RG_Material;
	if("rg_productive" == name)
		return Btn_RG_Productive;
	if("rg_food" == name)
		return Btn_RG_Food;
	if("rg_military" == name)
		return Btn_RG_Military;
	if("rg_physiological" == name)
		return Btn_RG_Physiological;
	if("rg_social" == name)
		return Btn_RG_Social;
	return Btn_None;
}

BuildingGroup VisualInformation::DefineBuildingGroup(const std::string& name)
{
	if("general" == name)
		return BType_General;
	if("military" == name)
		return BType_Military;
	if("resources" == name)
		return BType_Resources;
	if("processingRes" == name)
		return BType_ProcessingResources;
	return BType_Undefined;
}

CommandData* VisualInformation::GetLinkedCD(ButtonID btnID)
{
	CommandData *cData = NULL;
	int *atype = NULL;

	switch(btnID)
	{
	case Btn_Move:
		cData = new CommandData(Cmd_Move, CDParams_CharAction | CDParams_NeedPosition);
		break;
	case Btn_Attack:
		cData = new CommandData(Cmd_Attack, CDParams_CharAction | CDParams_NeedTarget);
		break;
	case Btn_Stop:
		cData = new CommandData(Cmd_Stop, CDParams_CharAction);
		break;
	case Btn_Create_Detinets:
		cData = new CommandData(Cmd_Create_Detinets, CDParams_CreateBuilding | CDParams_NeedPosition);
		break;
	case Btn_Create_Hizhina:
		cData = new CommandData(Cmd_Create_Hizhina, CDParams_CreateBuilding | CDParams_NeedPosition);
		break;
	case Btn_Create_Casern:
		cData = new CommandData(Cmd_Create_Casern, CDParams_CreateBuilding | CDParams_NeedPosition);
		break;
	case Btn_Create_Sawmill:
		cData = new CommandData(Cmd_Create_Sawmill, CDParams_CreateBuilding | CDParams_NeedPosition);
		break;
	case Btn_Create_StoneQuarry:
		cData = new CommandData(Cmd_Create_StoneQuarry, CDParams_CreateBuilding | CDParams_NeedPosition);
		break;
	case Btn_Create_WheatField:
		cData = new CommandData(Cmd_Create_WheatField, CDParams_CreateBuilding | CDParams_NeedPosition);
		break;
	case Btn_Create_VegetableGarden:
		cData = new CommandData(Cmd_Create_VegetableGarden, CDParams_CreateBuilding | CDParams_NeedPosition);
		break;
	case Btn_Create_Garden:
		cData = new CommandData(Cmd_Create_Garden, CDParams_CreateBuilding | CDParams_NeedPosition);
		break;
	case Btn_Create_Farm:
		cData = new CommandData(Cmd_Create_Farm, CDParams_CreateBuilding | CDParams_NeedPosition);
		break;
	case Btn_Create_Mill:
		cData = new CommandData(Cmd_Create_Mill, CDParams_CreateBuilding | CDParams_NeedPosition);
		break;
	case Btn_Create_Tannery:
		cData = new CommandData(Cmd_Create_Tannery, CDParams_CreateBuilding | CDParams_NeedPosition);
		break;
	case Btn_Create_Bakery:
		cData = new CommandData(Cmd_Create_Bakery, CDParams_CreateBuilding | CDParams_NeedPosition);
		break;
	case Btn_Create_Treasury:
		cData = new CommandData(Cmd_Create_Treasury, CDParams_CreateBuilding | CDParams_NeedPosition);
		break;
	case Btn_Create_RationStore:
		cData = new CommandData(Cmd_Create_RationStore, CDParams_CreateBuilding | CDParams_NeedPosition);
		break;
	case Btn_Create_StoreHouse:
		cData = new CommandData(Cmd_Create_StoreHouse, CDParams_CreateBuilding | CDParams_NeedPosition);
		break;
	}

	return cData;
}