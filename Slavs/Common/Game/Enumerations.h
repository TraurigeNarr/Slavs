#ifndef Enumerations_h
#define Enumerations_h

#include <string>

const std::string AbilityName = "ability";
const std::string Lucisus = "lucius";
const std::string DarkObidient = "darkObidient";

#pragma region ObjectNames
#pragma region Environment
const std::string tTerrain = "terrain";
const std::string tTree = "tree";
const std::string tGoldMine = "goldMine";
const std::string tSpawn = "spawn";
#pragma endregion

#pragma region Buildings
//Lucius
const std::string tDetinets = "detinets";
const std::string tCasern = "casern";
const std::string tHizhina = "hizhina";
const std::string tHovel = "hovel";
const std::string tHozdvor = "hozdvor";
const std::string tIzba = "izba";
const std::string tZemlyanka = "zemlyanka";
//Dark Obedient
const std::string tHOTD= "HOTD";
const std::string tHOD = "HOD";
const std::string tKurgan = "kurgan";
const std::string tLachuga = "lachuga";
const std::string tNora = "nora";
const std::string tPrison = "prison";
const std::string tSklep = "sklep";
#pragma endregion

#pragma region Characters
#pragma region Lucius
const std::string tPeasent = "peasent";
const std::string tVityaz = "vityaz";
const std::string tArcher = "archer";
const std::string tBogatyr = "bogatyr";
const std::string tVoinVedun = "voinVedun";
const std::string tDruid = "druid";
const std::string tVolhv = "volhv";
const std::string tOpyr = "opyr";
const std::string tVedma = "vedma";
const std::string tVedmak = "vedmak";

const std::string tDragon= "dragon";
#pragma endregion

#pragma region DarkObedient
const std::string tObedient= "obedient";
const std::string tUmrun = "umrun";
const std::string tVurdalak = "vurdalak";
const std::string tWerewolf= "werewolf";
const std::string tKoldun = "koldun";
const std::string tChernuiMag = "chernuiMag";
const std::string tUpyr = "upyr";
#pragma endregion
#pragma endregion
#pragma endregion
//constants, that defines bottom border of object type in enumeration
enum ObjectClass
{
	OC_EnvironmentObjects = 0,
	OC_BuildingObjects = 50,
	OC_CharacterObjects = 100
};

enum ObjectType
{
	OT_Terrain = OC_EnvironmentObjects, OT_Tree, OT_GoldMine, OT_Spawn, 
	OT_Detinets = OC_BuildingObjects, OT_House, OT_Casern, OT_Hizhina,
	OT_Sawmill, OT_StoneQuarry, OT_WheatField, OT_VegetableGarden, OT_Garden,
	OT_Farm, OT_Mill, OT_Tannery, OT_Bakery, OT_Treasury,
	OT_RationStore, OT_StoreHouse,

	OT_Hovel, OT_HozDvor, OT_Izba, OT_Zemlyanka,
	OT_HallOfTheDeath, OT_HovelOfDead, OT_Kurgan, OT_Lachuga, OT_Nora, OT_Prison, OT_Sklep,
  OT_Human = OC_CharacterObjects, OT_Worker, 
  OT_LumberJack, OT_Mason, OT_Gardener, OT_Farmer, OT_Miller, OT_Baker,OT_Skinner,
  OT_Vityaz, OT_Archer, OT_Bogatyr, OT_VoinVedun, OT_Druid, OT_Volhv, OT_Opyr, OT_Vedma, OT_Vedmak,
  OT_Obedient, OT_Umrun, OT_Vurdalak, OT_Werewolf, OT_Koldun, OT_ChernuiMag, OT_Upyr,
  OT_None
};

typedef int CommandDataParameters;
enum
{
	CDParams_CreateBuilding = 1 << 7,
	CDParams_CreateChar = 1 << 6,
	CDParams_CharAction = 1 << 5,
	CDParams_NeedTarget = 1 << 4,
	CDParams_NeedPosition = 1 << 3

};

enum CommandID
{
	Cmd_None = 1024,
	Cmd_Move = 0, Cmd_Stop, Cmd_Attack,	Cmd_Hold,
	Cmd_Create_Detinets, Cmd_Create_Hizhina, Cmd_Create_Casern,
	Cmd_Create_Sawmill, Cmd_Create_StoneQuarry, Cmd_Create_WheatField, 
	Cmd_Create_VegetableGarden, Cmd_Create_Garden, Cmd_Create_Farm, 
	Cmd_Create_Mill, Cmd_Create_Tannery, Cmd_Create_Bakery, 
	Cmd_Create_Treasury, Cmd_Create_RationStore, Cmd_Create_StoreHouse,

	Cmd_Create_Hovel, Cmd_Create_HozDvor, Cmd_Create_Izba, Cmd_Create_Zemlyanka,
	Cmd_Create_HOTD, Cmd_Create_HOD, Cmd_Create_Kurgan, Cmd_Create_Lachuga, Cmd_Create_Nora, Cmd_Create_Prison, Cmd_Create_Sklep,
	Cmd_Create_Peasent, Cmd_CreateChar_Vityaz, Cmd_CreateChar_Archer, Cmd_CreateChar_Bogatyr, Cmd_CreateChar_VoinVedun, Cmd_CreateChar_Druid, Cmd_CreateChar_Volhv, Cmd_CreateChar_Opyr, 
	Cmd_CreateChar_Vedma, Cmd_CreateChar_Vedmak, Cmd_CreateChar_Dragon,
	Cmd_Create_Obedient, Cmd_CreateChar_Umrun, Cmd_CreateChar_Vurdalak, Cmd_CreateChar_Werewolf, Cmd_CreateChar_Koldun, Cmd_CreateChar_ChernuiMag, Cmd_CreateChar_Upyr
};

enum Race
{
	RNone,
	RLucius,
	RDarkObedient
};

enum QueryMask
{
	QM_All = 1 << 7,
	QM_Own = 1 << 6,
	QM_OwnButUncontroller = 1 << 5,//objects like human, they lives in the houses but player can not control them
	QM_Allied = 1 << 4,
	QM_Enemy = 1 << 3,
	QM_Resources = 1 << 2,
	QM_Environment = 1 <<1
};

enum ObjectVisibility
{
	OBJECT_VISIBILITY_DEFAULT,
	OBJECT_VISIBLE,
	OBJECT_HIDDEN
};
#endif