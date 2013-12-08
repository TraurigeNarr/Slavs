#include "IGameObject.h"
#include "GameObjectState.h"

#include "../Utilities/TemplateFunctions.h"
#include "tinyxml.h"

#include <algorithm>

IGameObject::IGameObject(long ID, ObjectType otype, QueryMask iMask)
	: m_lID(ID), m_Type(otype), m_qmMask(iMask), m_bStateChanged(true), 
  m_bDestroyed(false), m_bSelected(false),
  m_ticks_for_components(0)
{}

IGameObject::~IGameObject()
{
	ClearVector(m_lComponents);
}

void IGameObject::TickForComponents()
{
	std::for_each(m_lComponents.begin(), m_lComponents.end(), [](IComponent* component)
	{
		component->TickPerformed();
	});
  ++m_ticks_for_components;
}

GameObjectState* IGameObject::GetState() const
{
	GameObjectState *GOState = new GameObjectState();
	GOState->lID = m_lID;
	if(m_bDestroyed)
	{
		GOState->iFlags |= GOF_Destroyed;
		return GOState;
	}
	GOState->oType = m_Type;
	GOState->iMask = m_qmMask;
	
	std::for_each(m_lComponents.begin(), m_lComponents.end(), [&GOState](IComponent* component)
		{
			component->GetState(*GOState);
	});
	//get the latest state. If the state changes we should set m_bStateChanged to true
	m_bStateChanged = false;
	return GOState;
}

ObjectType IGameObject::GetType() const
{
	return m_Type;
}

/************************************************************************/
/*                     Static functions                                 */
/************************************************************************/
std::string IGameObject::ToString(ObjectType oType)
{
	std::string s;
	switch(oType)
	{
	case OT_Detinets:
		{
			s = "Detinets";
			break;
		}
	case OT_Hizhina:
		{
			s = "Hizhina";
			break;
		}
	case OT_Casern:
		{
			s = "Casern";
			break;
		}
	case OT_Sawmill:
		{
			s = "Sawmill";
			break;
		}
	case OT_StoneQuarry:
		{
			s = "Stone quarry";
			break;
		}
	case OT_WheatField:
		{
			s = "Wheat field";
			break;
		}
	case OT_VegetableGarden:
		{
			s = "Vegetable garden";
			break;
		}
	case OT_Garden:
		{
			s = "Garden";
			break;
		}
	case OT_Farm:
		{
			s = "Farm";
			break;
		}
	case OT_Mill:
		{
			s = "Mill";
			break;
		}
	case OT_Tannery:
		{
			s = "Tannery";
			break;
		}
	case OT_Bakery:
		{
			s = "Bakery";
			break;
		}
	case OT_Treasury:
		{
			s = "Treasury";
			break;
		}
	case OT_RationStore:
		{
			s = "Ration store";
			break;
		}
	case OT_StoreHouse:
		{
			s = "Store house";
			break;
		}
	//Characters
	case OT_Human:
		{
			s = "Human";
			break;
		}
	case OT_Worker:
		{
			s = "Worker";
			break;
		}
  case OT_LumberJack:
    {
      s = "Lumberjack";
      break;
    }
  case OT_Mason:
    {
      s = "Mason";
      break;
    }
  case OT_Miller:
    {
      s = "Miller";
      break;
    }
  case OT_Farmer:
    {
      s = "Farmer";
      break;
    }
  case OT_Skinner:
    {
      s = "Skinner";
      break;
    }
  case OT_Gardener:
    {
      s = "Gardener";
      break;
    }
  case OT_Baker:
    {
      s = "Baker";
      break;
    }
	}
	return s;
}

ObjectType IGameObject::GetGameObjectType(const std::string& oType)
{
#pragma region Environment
	if(tTerrain == oType)
		return OT_Terrain;
	if(tSpawn == oType)
		return OT_Spawn;
	if(tTree == oType)
		return OT_Tree;
	if(tGoldMine == oType)
		return OT_GoldMine;
#pragma endregion

	if(tDetinets == oType)
		return OT_Detinets;
	if(tHizhina == oType)
		return OT_Hizhina;
	if(tCasern == oType)
		return OT_Casern;
	if("sawmill" == oType)
		return OT_Sawmill;
	if("stoneQuarry" == oType)
		return OT_StoneQuarry;
	if("wheatField" == oType)
		return OT_WheatField;
	if("vegetableGarden" == oType)
		return OT_VegetableGarden;
	if("garden" == oType)
		return OT_Garden;
	if("farm" == oType)
		return OT_Farm;
	if("mill" == oType)
		return OT_Mill;
	if("tannery" == oType)//кожевня
		return OT_Tannery;
	if("bakery" == oType)
		return OT_Bakery;
	if("treasury" == oType)
		return OT_Treasury;
	if("rationStore" == oType)
		return OT_RationStore;
	if("storeHouse" == oType)
		return OT_StoreHouse;

#pragma region Buildings
	//Lucius
	if(tHovel == oType)
		return OT_Hovel;
	if(tHozdvor == oType)
		return OT_HozDvor;
	if(tIzba == oType)
		return OT_Izba;
	if(tZemlyanka == oType)
		return OT_Zemlyanka;
	if("rationStore" == oType)
		return OT_RationStore;
	//Dark Obedient
	if(tHOTD == oType)
		return OT_HallOfTheDeath;
	if(tHOD == oType)
		return OT_HovelOfDead;
	if(tKurgan == oType)
		return OT_Kurgan;
	if(tLachuga == oType)
		return OT_Lachuga;
	if(tNora == oType)
		return OT_Nora;
	if(tPrison == oType)
		return OT_Prison;
	if(tSklep == oType)
		return OT_Sklep;
#pragma endregion

	if("human" == oType)
		return OT_Human;
	if("archer" == oType)
		return OT_Archer;
	if("worker" == oType)
		return OT_Worker;

  if("lumberjack" == oType)
    return OT_LumberJack;
  if("mason" == oType)
    return OT_Mason;
  if("farmer" == oType)
    return OT_Farmer;
  if("gardener" == oType)
    return OT_Gardener;
  if("miller" == oType)
    return OT_Miller;
  if("baker" == oType)
    return OT_Baker;
  if("skinner" == oType)
    return OT_Skinner;

#pragma region Characters
	//Lucius
	if(tPeasent == oType)
		return OT_Human;
	if(tArcher == oType)
		return OT_Archer;
	//Dark Obedient
	if(tObedient == oType)
		return OT_Obedient;
	if(tWerewolf == oType)
		return OT_Werewolf;
#pragma endregion
	return OT_None;
}

ObjectType IGameObject::GetGameObjectType(CommandID cmdID)
{
	switch(cmdID)
	{
	case Cmd_Create_Detinets:
		return OT_Detinets;
	case Cmd_Create_Hizhina:
		return OT_Hizhina;
	case Cmd_Create_Casern:
		return OT_Casern;
	case Cmd_Create_Sawmill:
		return OT_Sawmill;
	case Cmd_Create_StoneQuarry:
		return OT_StoneQuarry;
	case Cmd_Create_WheatField:
		return OT_WheatField;
	case Cmd_Create_VegetableGarden:
		return OT_VegetableGarden;
	case Cmd_Create_Garden:
		return OT_Garden;
	case Cmd_Create_Farm:
		return OT_Farm;
	case Cmd_Create_Mill:
		return OT_Mill;
	case Cmd_Create_Tannery:
		return OT_Tannery;
	case Cmd_Create_Bakery:
		return OT_Bakery;
	case Cmd_Create_Treasury:
		return OT_Treasury;
	case Cmd_Create_RationStore:
		return OT_RationStore;
	case Cmd_Create_StoreHouse:
		return OT_StoreHouse;
	}
	return OT_None;
}