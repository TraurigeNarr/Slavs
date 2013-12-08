#ifndef ClientEnums_h
#define ClientEnums_h
#include <string>

#pragma region CommandsName

#pragma region Characters
//common
const std::string MoveName = "move";
const std::string StopName = "stop";
const std::string AttackName = "attack";
const std::string HoldName = "hold";
const std::string ChooseBuilding = "build";
const std::string RejectChoosingBuilding = "reject_build";
//Lucius
const std::string CreateDetinets = "create_detinets";
const std::string CreateCasern = "create_casern";
const std::string CreateHizhina = "create_hizhina";
const std::string CreateHovel = "create_hovel";
const std::string CreateHozDvor = "create_hozDvor";
const std::string CreateIzba = "create_izba";
const std::string CreateZemlyanka = "create_zemlyanka";
//Dark Obedient
const std::string CreateHOTD = "create_HOTD";
const std::string CreateHOD = "create_HOD";
const std::string CreateKurgan = "create_kurgan";
const std::string CreateLachuga = "create_lachuga";
const std::string CreateNora = "create_nora";
const std::string CreatePrison = "create_prison";
const std::string CreateSklep = "create_sklep";
#pragma endregion

#pragma region Buildings

#pragma region Lucius
const std::string CreatePeasent = "create_peasent";
const std::string CreateVityaz = "create_vityaz";
const std::string CreateArcher = "create_archer";
const std::string CreateBogatyr = "create_bogatyr";
const std::string CreateVoinVedun = "create_voinVedun";
const std::string CreateDruid = "create_druid";
const std::string CreateVolhv = "create_volhv";
const std::string CreateOpyr = "create_opyr";
#pragma endregion

const std::string CreateDragon = "create_dragon";
const std::string CreateVedma = "create_vedma";
const std::string CreateVedmak = "create_vedmak";


#pragma region DarkObedient
const std::string CreateObedient = "create_obedient";
const std::string CreateUmrun = "create_umrun";
const std::string CreateVurdalak = "create_vurdalak";
const std::string CreateWerewolf = "create_werewolf";
const std::string CreateKoldun = "create_koldun";
const std::string CreateChernuiMag = "create_chernuiMag";
const std::string CreateUpyr = "create_upyr";
#pragma endregion

#pragma endregion

#pragma endregion

#pragma region CommandsName
const std::string BGroup_general = "bType_general";
const std::string BGroup_military = "bType_military";
#pragma endregion

enum ButtonID
{ 
	Btn_None = 1024,
	Btn_UI_Menu = 0, Btn_UI_Options,
	Btn_UI_CreateLevel, Btn_UI_MapChanged, 
	Btn_UI_StartGame, Btn_UI_Confirm, 
	Btn_UI_OK, Btn_UI_Cancel, Btn_UI_Quit, Btn_UI_Help,

	Btn_Move, Btn_Stop, Btn_Attack,	Btn_Hold, Btn_ChooseBuilding, Btn_RejectChoosingBuilding,

	Btn_RG_Material, Btn_RG_Productive, Btn_RG_Food, Btn_RG_Military, Btn_RG_Physiological, Btn_RG_Social,


	Btn_BGroup_General, Btn_BGroup_Military, Btn_BGroup_Resources, Btn_BGroup_ProcessingResources,
	Btn_Create_Detinets, Btn_Create_Hizhina, Btn_Create_Casern,
	Btn_Create_Sawmill, Btn_Create_StoneQuarry, Btn_Create_WheatField, Btn_Create_VegetableGarden, Btn_Create_Garden,
	Btn_Create_Farm, Btn_Create_Mill, Btn_Create_Tannery, Btn_Create_Bakery, Btn_Create_Treasury,
	Btn_Create_RationStore, Btn_Create_StoreHouse,

	Btn_Create_Hovel, Btn_Create_HozDvor, Btn_Create_Izba, Btn_Create_Zemlyanka,
	Btn_Create_HOTD, Btn_Create_HOD, Btn_Create_Kurgan, Btn_Create_Lachuga, Btn_Create_Nora, Btn_Create_Prison, Btn_Create_Sklep,
	Btn_Create_Peasent, Btn_CreateChar_Vityaz, Btn_CreateChar_Archer, Btn_CreateChar_Bogatyr, Btn_CreateChar_VoinVedun, Btn_CreateChar_Druid, Btn_CreateChar_Volhv, Btn_CreateChar_Opyr, 
	Btn_CreateChar_Vedma, Btn_CreateChar_Vedmak, Btn_CreateChar_Dragon,
	Btn_Create_Obedient, Btn_CreateChar_Umrun, Btn_CreateChar_Vurdalak, Btn_CreateChar_Werewolf, Btn_CreateChar_Koldun, Btn_CreateChar_ChernuiMag, Btn_CreateChar_Upyr
};

enum BuildingGroup
{
	BType_General = 1442,
	BType_Military,
	BType_Resources,
	BType_ProcessingResources,
	BType_Undefined,
};

enum CommandToUI
{
	Cmd_UI_AddMap, Cmd_UI_ChangeMode, Cmd_UI_GoToMenu,
	Cmd_UI_SingleSelection, Cmd_UI_MultipleSelection, Cmd_UI_GOStateChanged,
	Cmd_UI_Deselect, Cmd_UI_ResourcesChanged,
};

enum GameStateModes
{
	GS_Idle, GS_PlacingBuilding, GS_GettingTarget,
};

#endif