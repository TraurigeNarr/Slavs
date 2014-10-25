#include "GameResources.h"
#include "../Utilities/TemplateFunctions.h"
#include "tinyxml.h"

#include <cassert>
#include <algorithm>

GameResourceContainer::GameResourceContainer(GameResourceType grType, size_t maxNum, size_t num /* = 0 */)
	: m_ResType(grType), m_uiMaxNumber(maxNum), m_bStateChanged(true), m_uiCurrentNumber(0)
{
}

GameResourceContainer::~GameResourceContainer()
{
	while(!m_lResourceList.empty())
	{
		delete m_lResourceList.back();
		m_lResourceList.pop_back();
	}
}

void GameResourceContainer::TickPerformed()
{
	std::list<GameResourceBox*>::iterator iter = m_lResourceList.begin();
	std::list<GameResourceBox*>::iterator iterEnd = m_lResourceList.end();

	for(;iter != iterEnd;)
	{
		(*iter)->TickPerformed();
		if(!(*iter)->Available())
		{
			delete (*iter);
			iter = m_lResourceList.erase(iter);
		}
		else
			++iter;
	}	
}

bool GameResourceContainer::AddResource(GameResourceBox* gr)
{
	UpdateAccordingToList();
	//how many we can add
	if(gr->GetGRType() != m_ResType || m_uiCurrentNumber >= m_uiMaxNumber)
		return false;

	size_t canAdd = m_uiMaxNumber - m_uiCurrentNumber;

	size_t added = 0;

	if(canAdd >= gr->GetNumber())
		added = gr->GetNumber();
	else
		added = canAdd;

	m_lResourceList.push_back(new GameResourceBox(gr->GetGRType(), added, gr->GetExpireCycles(), gr->Price()));
	gr->GetResources(added);
	UpdateAccordingToList();
	m_bStateChanged = true;
	return gr->Empty();
}

size_t GameResourceContainer::PeekResource(size_t number)
{
	//refresh current number
	UpdateAccordingToList();
	if(0 != m_uiCurrentNumber)
	{
		size_t peeked_number = 0;
		while(!m_lResourceList.empty())
		{
			peeked_number += (*m_lResourceList.begin())->GetResources(number);
			if((*m_lResourceList.begin())->Empty())
			{
				delete m_lResourceList.front();
				m_lResourceList.pop_front();
			}
      if (peeked_number == number)
        break;
		}
		m_bStateChanged = true;

		return peeked_number;
	}
	return 0;
}

bool GameResourceContainer::AvailableToAdd()
{
	UpdateAccordingToList();

	return m_uiCurrentNumber < m_uiMaxNumber;
}

size_t GameResourceContainer::GetResourceNumber() const
{
	size_t number = 0;
	std::list<GameResourceBox*>::const_iterator iter = m_lResourceList.begin();
	while(iter != m_lResourceList.end())
	{
		number += (*iter)->GetNumber();
		++iter;
	}
	return number;
}

void GameResourceContainer::UpdateAccordingToList() const
  {
  if (m_bStateChanged)
    m_uiCurrentNumber = GetResourceNumber();
  }

void GameResourceContainer::ExpandContainer(size_t number)
{
	 m_uiMaxNumber += number; 
	
	 m_bStateChanged = true;
}

std::list<GameResourceBox*>& GameResourceContainer::GetResourceList()
{
	return m_lResourceList;
}
/*
------------------------------------------------------------------------------
							Serialize/Deserialize
------------------------------------------------------------------------------
*/
size_t GameResourceContainer::Serialize(char* buf_end, size_t size) const
{
	if(NeededSize() > size)
		return 0;
	this->UpdateAccordingToList();
	ToChar(m_ResType, buf_end, sizeof(m_ResType));
	buf_end += sizeof(m_ResType);
	ToChar(m_uiCurrentNumber, buf_end, sizeof(m_uiCurrentNumber));
	buf_end += sizeof(m_uiCurrentNumber);
	ToChar(m_uiMaxNumber, buf_end, sizeof(m_uiMaxNumber));
	buf_end += sizeof(m_uiMaxNumber);
	m_bStateChanged = false;
	return NeededSize();
}

size_t GameResourceContainer::Deserialize(char* buf_end)
{
	assert(buf_end && 
		"<GameResourceContainer::Deserialize>: buffer for deserialize is NULL");
	m_ResType			= (GameResourceType)FromChar<int>(buf_end);
	m_uiCurrentNumber	= FromChar<int>(buf_end + sizeof(GameResourceType));
	m_uiMaxNumber		= FromChar<int>(buf_end + sizeof(GameResourceType) + sizeof(int));
	return NeededSize();
}
/*
------------------------------------------------------------------------------
							Static Methods
------------------------------------------------------------------------------
*/
size_t GameResourceContainer::NeededSize()
{
	return 2*sizeof(size_t) + sizeof(GameResourceType);
}

/************************************************************************/
/*                       Game Resource Box Class                        */
/************************************************************************/

GameResourceBox::GameResourceBox(GameResourceType grType, size_t number, size_t iExpireCycles, size_t i_price, TiXmlElement* i_config_element /* = nullptr*/, float fQuality /* = 1 */)
	: m_ResType(grType), m_iExpireCycles(iExpireCycles), m_price(i_price),
	m_iCurrentCycle(0), m_fQuality(fQuality), m_uiResNumber(number)
{
	m_ResGroup = GameResourceBox::DefineResGroup(m_ResType);
  ParseElelement(i_config_element);
}

size_t GameResourceBox::GetResources(size_t number)
{
	if(0 == number)
		return 0;

	if(m_uiResNumber >= number)
	{
		m_uiResNumber -= number;
		return number;
	}

	size_t wereResources = m_uiResNumber;
	m_uiResNumber = 0;
	return wereResources;
}

void GameResourceBox::ParseElelement(const TiXmlElement* i_config_element)
{

}

void GameResourceBox::Clear()
{
	m_iCurrentCycle = 0;
	m_uiResNumber = 0;
}

void GameResourceBox::TickPerformed()
{

}
/*
------------------------------------------------------------------------------
							Static Methods
------------------------------------------------------------------------------
*/
GameResourceType GameResourceBox::DefineResType(const std::string& name)
{
	if("tree" == name)
		return GR_Tree;
	if("wood" == name)
		return GR_Wood;
	if("stone" == name)
		return GR_Stone;
	if("ironOre" == name)
		return GR_IronOre;
	if("wheat" == name)
		return GR_Wheat;
	if("wheatFlour" == name)
		return GR_WheatFlour;
	if("bread" == name)
		return GR_Bread;
	if("fruits" == name)
		return GR_Fruits;
	if("vegetables" == name)
		return GR_Vegetables;
	if("meat" == name)
		return GR_Meat;
	if("milk" == name)
		return GR_Milk;
	if("cheese" == name)
		return GR_Cheese;
	if("wool" == name)
		return GR_Wool;
	if("skin" == name)
		return GR_Skin;
	if("population" == name)
		return GR_Population;
  if("money" == name)
    return GR_Money;
	assert("There is no such resource");
	return GR_None;
}

std::string GameResourceBox::GetResTypeString(GameResourceType grType)
{
	switch(grType)
	{
	case GR_Tree:
		return "Tree";
	case GR_Wood:
		return "Wood";
	case GR_Stone:
		return "Stone";
	case GR_IronOre:
		return "Iron ore";
	case GR_Wheat:
		return "Wheat";
	case GR_WheatFlour:
		return "Wheat flour";
	case GR_Bread:
		return "Bread";
	case GR_Fruits:
		return "Fruits";
	case GR_Vegetables:
		return "Vegetables";
	case GR_Meat:
		return "Meat";
	case GR_Milk:
		return "Milk";
	case GR_Cheese:
		return "Cheese";
	case GR_Wool:
		return "Wool";
	case GR_Skin:
		return "Skin";
	case GR_Population:
		return "Population";
  case GR_Money:
    return "Money";
	}
	return "";
}

GameResourceGroup GameResourceBox::DefineResGroup(GameResourceType grType)
{
	switch(grType)
	{
	case GR_Tree:
	case GR_Wood:
	case GR_Stone:
	case GR_IronOre:
		return GRG_Material;
	case GR_Skin:
	case GR_Wool:
  case GR_WheatFlour:
  case GR_Wheat:
		return GRG_Productive;
	case GR_Bread:
	case GR_Fruits:
	case GR_Vegetables:
	case GR_Meat:
	case GR_Milk:
	case GR_Cheese:
		return GRG_Food;
	case GR_Population:
  case GR_Money:
		return GRG_Social;
	default:
		return GRG_None;
		break;
	}
	return GRG_None;
}

/************************************************************************/
/*                 GeneralResourceInfo                                  */
/************************************************************************/

size_t GeneralResourceInfo::NeededSize()
{
	return 2*sizeof(size_t) + sizeof(GameResourceType);
}

size_t GeneralResourceInfo::Serialize(char* buf_end, size_t size) const
{
	if(NeededSize() > size)
		return 0;
	ToChar(m_res_type, buf_end, sizeof(m_res_type));
	buf_end += sizeof(m_res_type);
	ToChar(uiCurrentResources, buf_end, sizeof(uiCurrentResources));
	buf_end += sizeof(uiCurrentResources);
	ToChar(uiMaxResources, buf_end, sizeof(uiMaxResources));
	buf_end += sizeof(uiMaxResources);
	bHasChanges = false;
	return NeededSize();
}

size_t GeneralResourceInfo::Deserialize(char* buf_end)
{
	assert(buf_end && 
		"<GameResourceContainer::Deserialize>: buffer for deserialize is NULL");
	m_res_type				= (GameResourceType)FromChar<int>(buf_end);
	uiCurrentResources	= FromChar<int>(buf_end + sizeof(GameResourceType));
	uiMaxResources		= FromChar<int>(buf_end + sizeof(GameResourceType) + sizeof(int));
	return NeededSize();
}