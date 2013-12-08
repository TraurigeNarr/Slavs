#include "SBuildingComp.h"
//server
#include "SBuildingComp.h"
#include "SGameObject.h"
#include "misc\ServerEnums.h"
//common
#include <Game/GameObjectState.h>
#include <Patterns/Telegram.h>
#include <Utilities/XmlUtilities.h>
//standard
#include <cassert>

SBuildingComp::SBuildingComp(SGameObject* owner, const TiXmlElement* componentElement)
	: m_pOwner(owner)
{
	assert(componentElement &&
		"<SBuilding::SBuilding>: NULL parameter of config element.");
	ParseElelement(componentElement);
}

SBuildingComp::~SBuildingComp()
{
}

bool SBuildingComp::HandleMessage(const Telegram& msg)
{
	return false;
}

void SBuildingComp::GetState(GameObjectState& goState) const
{

}

void SBuildingComp::ParseElelement(const TiXmlElement* componentElement)
{
	m_iHP		  = XmlUtilities::GetIntAttribute(componentElement, "hp", 0);
	m_iArmor	= XmlUtilities::GetIntAttribute(componentElement, "armor", 0);

	m_iCurrentHP = m_iHP;
}