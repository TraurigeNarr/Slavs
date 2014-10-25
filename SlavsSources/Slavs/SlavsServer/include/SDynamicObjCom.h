/*
--------------------------------------------------------
Class that responds for objects that have position
and can move. Also component has velocity, heading, side vector (perpendicular to heading). This component may include
all characters (objects that can move with some speed).
--------------------------------------------------------
*/
#ifndef SDynamicObjCom_h
#define SDynamicObjCom_h

#include <Game/IComponent.h>
#include <Math/Vector2D.h>

#include <Math\Matrix2D.h>

//////////////////////////////////////////////////////////////////////////

class Telegram;
class SGameObject;
class TiXmlElement;
class SteeringBehaviors;

//////////////////////////////////////////////////////////////////////////

class SDynamicObjCom : public IComponent
{
public:
	SDynamicObjCom(SGameObject* owner, const TiXmlElement* componentElement,  const Vector2D* initialPosition = 0);
	~SDynamicObjCom();

	void				TickPerformed();
	bool				HandleMessage(const Telegram& msg);

	//accessors
	SGameObject*		GetOwner() const { return m_pOwner; }
	SteeringBehaviors*	GetSteering() const { return m_pSteering; }
	Vector2D			GetVelocity()const{return m_vVelocity;}
	Vector2D			GetTarget()const{ return m_vTarget; }
	Vector2D			GetSide()const{return m_vSide;}
	float				GetMaxSpeed()const{return m_fMaxSpeed;}
	float				GetMaxForce()const{return m_fMaxForce;}
	float				GetSpeed()const{return m_vVelocity.Length();}
	float				GetSpeedSq()const{return m_vVelocity.LengthSq();}
	Vector2D			GetHeading()const{return m_vHeading;}
	float				GetMaxTurnRate()const{return m_fMaxTurnRate;}
	void				GetState(GameObjectState&) const;
	const Vector2D&		GetPosition() const { return m_vPosition; }
	float				GetBRadius()const{ return m_fBoundingRadius; }

	void				SetTarget(const Vector2D& target);
	void				SetBRadius(float val);
	void				SetVelocity(const Vector2D& NewVel);
	void				SetMaxSpeed(float new_speed);
	void				SetMaxForce(float mf);
	void				SetHeading(Vector2D new_heading);
	void				SetMaxTurnRate(float val);

	bool				IsSpeedMaxedOut()const{return m_fMaxSpeed*m_fMaxSpeed >= m_vVelocity.LengthSq();}
	bool				RotateHeadingToFacePosition(Vector2D target);

	bool				bTag;
protected:
	void				ParseElelement(const TiXmlElement* componentElement);

	SGameObject*		m_pOwner;

	Vector2D			m_vPosition;
	Vector2D			m_vVelocity;
	Vector2D			m_vTarget;

	//a normalized vector pointing in the direction the entity is heading
	Vector2D			m_vHeading;
	//a vector perpendicular to the heading vector
	Vector2D			m_vSide;
	//--------------------Get from xml------------------------
	//the length of this object's bounding radius
	float				m_fBoundingRadius;
	//the maximum speed at which this entity may travel
	float				m_fMaxSpeed;
	//the maximum force this entity can produce to power itself
	float				m_fMaxForce;
	//the maximum rate (radians per second) at which this vehicle can rotate
	float				m_fMaxTurnRate;
	//steering behavior class
	SteeringBehaviors*	m_pSteering;
};


//--------------------------- RotateHeadingToFacePosition ---------------------
//
//  given a target position, this method rotates the entity's heading and
//  side vectors by an amount not greater than m_dMaxTurnRate until it
//  directly faces the target.
//
//  returns true when the heading is facing in the desired direction
//-----------------------------------------------------------------------------
inline bool SDynamicObjCom::RotateHeadingToFacePosition(Vector2D target)
{
	Vector2D toTarget = Vec2DNormalize(target - m_vPosition);

	//first determine the angle between the heading vector and the target
	float angle = acos(m_vHeading.Dot(toTarget));

	//return true if the player is facing the target
	if (angle < 0.00001) return true;

	//clamp the amount to turn to the max turn rate
	if (angle > m_fMaxTurnRate) angle = m_fMaxTurnRate;

	//The next few lines use a rotation matrix to rotate the player's heading
	//vector accordingly
	Matrix2D RotationMatrix;

	//notice how the direction of rotation has to be determined when creating
	//the rotation matrix
	RotationMatrix.Rotate(angle * m_vHeading.Sign(toTarget));	
	RotationMatrix.TransformVector2Ds(m_vHeading);
	RotationMatrix.TransformVector2Ds(m_vVelocity);

	//finally recreate m_vSide
	m_vSide = m_vHeading.Perp();

	return false;
}

#endif