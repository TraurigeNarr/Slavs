#include "SStaticObjCom.h"
//server
#include "SGameObject.h"
#include "SGameContext.h"
//common
#include <Game/GameObjectState.h>
#include <Patterns/Telegram.h>
#include <Utilities/XmlUtilities.h>
//standard
#include <cassert>

SStaticObjCom::SStaticObjCom(SGameObject* owner, const TiXmlElement* componentElement, const Vector2D* initialPosition /* = 0 */)
	: m_pOwner(owner)
{
	if(0 != initialPosition)
		m_vPosition = *initialPosition;
	assert(componentElement &&
		"SHouseComponent::SHouseComponent: NULL parameter of config element.");
	ParseElelement(componentElement);
	owner->GetContext()->AddObstacle(this);
}

SStaticObjCom::~SStaticObjCom()
{
}

void SStaticObjCom::TickPerformed()
{

}

bool SStaticObjCom::HandleMessage(const Telegram& msg)
{
	return false;
}

void SStaticObjCom::GetState(GameObjectState &state) const
{
	//set flag for position
	state.iFlags |= GOF_Position;

	state.pPosition = new Vector2D(m_vPosition);
}

void SStaticObjCom::ParseElelement(const TiXmlElement* componentElement)
{
	m_fBoundingRadius	= XmlUtilities::GetRealAttribute(componentElement, "bRadius");
}