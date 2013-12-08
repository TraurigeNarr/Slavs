#include "SStoreHouseCom.h"
//server
#include "IController.h"
#include "SGameObject.h"
#include "SResourceManager.h"
//additional components
#include "SBuildingComp.h"

#include "Management/Goverment.h"
#include "Management/IEconomyManager.h"
#include "Management/IStoreSystem.h"

//common
#include <Game/GameObjectState.h>
#include <Patterns/Telegram.h>
#include <Utilities/XmlUtilities.h>
//standard
#include <algorithm>
#include <string>
#include <cassert>

SStoreHouseCom::SStoreHouseCom(SGameObject* owner, const TiXmlElement* componentElement)
	: m_pOwner(owner), m_pBuilding(NULL)
{
	assert(componentElement &&
		"SHouseComponent::SHouseComponent: NULL parameter of config element.");
	ParseElelement(componentElement);
  owner->GetOwner()->GetGoverment().GetEconomyManager()->GetStoreSystem()->Register(this);
}

SStoreHouseCom::~SStoreHouseCom()
{
  m_pOwner->GetOwner()->GetGoverment().GetEconomyManager()->GetStoreSystem()->Remove(this);
}

void SStoreHouseCom::GetState(GameObjectState& state) const
{
	state.iFlags |= GOF_Resources;

	state.vContainers = m_vResources;
}

/*
std::shared_ptr<GameResourceContainer> SStoreHouseCom::GetResource(GameResourceType grType) const
{
  TResourceContainers::const_iterator it = std::find(m_vResources.begin(), m_vResources.end(), [grType](std::shared_ptr<GameResourceContainer> container) -> bool
  {
    return container->GetResType() == grType;
  });
	
	return *it;
}*/

bool SStoreHouseCom::AddResource(GameResourceBox* gameResource) const
{
	bool added = false;
	for(size_t i = 0; i < m_vResources.size(); ++i)
	{
		if(m_vResources[i]->AddResource(gameResource))
		{
			added = true;
		}
		if(gameResource->GetNumber() == 0)
		{
			break;
		}
	}
	if(added)
		m_pOwner->StateChanged();
	return added;
}

size_t SStoreHouseCom::PeekResources(GameResourceType grType, size_t number)
{
	size_t peekedNumber = 0;
	std::for_each(m_vResources.begin(), m_vResources.end(), [grType, number, &peekedNumber](std::shared_ptr<GameResourceContainer> resource)
	{
		if(resource->GetResType() == grType)
		{
			peekedNumber += resource->PeekResource(number - peekedNumber);
			assert(peekedNumber <= number &&
				"<SStoreHouseCom::PeekResources>: Can not get more resources than needed.");
			if(peekedNumber == number)
				return;
		}
	});
	m_pOwner->StateChanged();
	return peekedNumber;
}

void SStoreHouseCom::ParseElelement(const TiXmlElement* componentElement)
{
	std::string elementName = "";

	const TiXmlElement* childElement = 0;

	while (childElement = XmlUtilities::IterateChildElements(componentElement, childElement))
	{
		elementName = childElement->Value();

		if("resource" == elementName)
		{
			int maxNum	= XmlUtilities::GetIntAttribute(childElement, "maxNumber", 0);
			GameResourceType grType = GameResourceBox::DefineResType(XmlUtilities::GetStringAttribute(childElement, "type", ""));
      if(grType != GR_None)
			m_vResources.push_back(std::make_shared<GameResourceContainer>(GameResourceContainer(grType, maxNum)));
		}
	}
}

bool SStoreHouseCom::Probe()
{
	//checks for SBuilding
	assert(m_pOwner->GetComponent<SBuildingComp>() &&
		"<SStoreHouseCom::Probe>: NULL pointer of SDynamicObjCom.");
	m_pBuilding = m_pOwner->GetComponent<SBuildingComp>();
	return true;
}
