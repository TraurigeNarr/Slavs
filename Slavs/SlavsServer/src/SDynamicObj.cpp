#include "SDynamicObjCom.h"
#include "SteeringBehaviors.h"
//server
#include "SGameContext.h"
#include "SGameObject.h"
//common
#include <Game/GameObjectState.h>
#include <Patterns/Telegram.h>
#include <Utilities/XmlUtilities.h>
//standard
#include <cassert>

SDynamicObjCom::SDynamicObjCom(SGameObject* owner, const TiXmlElement* componentElement, const Vector2D* initialPosition /* = 0 */)
	: m_pOwner(owner)
{
	if(0 != initialPosition)
		m_vPosition = *initialPosition;
	assert(componentElement &&
		"SHouseComponent::SHouseComponent: NULL parameter of config element.");
	ParseElelement(componentElement);
	m_vVelocity.Zero();
	m_vTarget.Zero();
	m_pSteering = new SteeringBehaviors(this);
	//switch on general behaviors
	m_pSteering->ObstacleAvoidanceOn();
	m_pSteering->SeparationOn();
	owner->GetContext()->AddDynamicObject(this);
}

SDynamicObjCom::~SDynamicObjCom()
{
	if(NULL != m_pSteering)
		delete m_pSteering;
}

void SDynamicObjCom::TickPerformed()
{
	if(m_vTarget.isZero())
		return;
	Vector2D SteeringForce;
	
	SteeringForce = m_pSteering->Calculate();

	m_vVelocity += SteeringForce;
	m_vVelocity.Truncate(m_fMaxSpeed);

	m_vPosition += m_vVelocity;

	//update heading
	if(0.00000001f < m_vVelocity.LengthSq())
	{
		m_vHeading = Vec2DNormalize(m_vVelocity);
		m_vSide = m_vHeading.Perp();
	}

	m_pOwner->StateChanged();
}

bool SDynamicObjCom::HandleMessage(const Telegram& msg)
{
	return false;
}

/************************************************************************/
/*				Setters:change state of the owner object				*/
/************************************************************************/
void SDynamicObjCom::SetTarget(const Vector2D& target)
{
	m_vTarget = target;
	m_pSteering->SetTarget(target);
	m_pOwner->StateChanged();
}

void SDynamicObjCom::SetBRadius(float val)
{
	m_fBoundingRadius = val;
	m_pOwner->StateChanged();
}

void SDynamicObjCom::SetVelocity(const Vector2D& NewVel)
{
	m_vVelocity = NewVel;
	m_pOwner->StateChanged();
}

void SDynamicObjCom::SetMaxSpeed(float new_speed)
{
	m_fMaxSpeed = new_speed;
	m_pOwner->StateChanged();
}

void SDynamicObjCom::SetMaxForce(float mf)
{
	m_fMaxForce = mf;
	m_pOwner->StateChanged();
}

//------------------------- SetHeading ----------------------------------------
//
//  first checks that the given heading is not a vector of zero length. If the
//  new heading is valid this function sets the entity's heading and side 
//  vectors accordingly
//-----------------------------------------------------------------------------
inline void SDynamicObjCom::SetHeading(Vector2D new_heading)
{
	assert( (new_heading.LengthSq() - 1.0) < 0.00001);

	m_vHeading = new_heading;

	//the side vector must always be perpendicular to the heading
	m_vSide = m_vHeading.Perp();
	m_pOwner->StateChanged();
}

void SDynamicObjCom::SetMaxTurnRate(float val)
{
	m_fMaxTurnRate = val;
	m_pOwner->StateChanged();
}

void SDynamicObjCom::GetState(GameObjectState& state) const
{
	//set flag for position
	state.iFlags |= GOF_Position;

	state.pPosition = new Vector2D(m_vPosition);
}

void SDynamicObjCom::ParseElelement(const TiXmlElement* componentElement)
{
	m_fBoundingRadius	= XmlUtilities::GetRealAttribute(componentElement, "bRadius");
	m_fMaxSpeed			= XmlUtilities::GetRealAttribute(componentElement, "maxSpeed");
	m_fMaxForce			= XmlUtilities::GetRealAttribute(componentElement, "maxForce");
	m_fMaxTurnRate		= XmlUtilities::GetRealAttribute(componentElement, "maxTurnRate");
}