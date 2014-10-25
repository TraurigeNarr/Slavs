/*
--------------------------------------------------------
Class that responds for objects that have position
but cannot move. This component may include buildings or
gold mines, trees and objects with common types.
--------------------------------------------------------
*/
#ifndef SStaticObjCom_h
#define SStaticObjCom_h

#include <Game/IComponent.h>
#include <Math/Vector2D.h>

//////////////////////////////////////////////////////////////////////////

class Telegram;
class SGameObject;
class TiXmlElement;

//////////////////////////////////////////////////////////////////////////

class SStaticObjCom : public IComponent
{
public:
	SStaticObjCom(SGameObject* owner, const TiXmlElement* componentElement, const Vector2D* initialPosition = 0);
	~SStaticObjCom();

	void			TickPerformed();
	bool			HandleMessage(const Telegram& msg);

	SGameObject*	GetOwner() const { return m_pOwner; }
	void			GetState(GameObjectState&) const;
	const Vector2D&	GetPosition() const { return m_vPosition; }
	float			GetBRadius()const{ return m_fBoundingRadius; }

	void			SetBRadius(float val) { m_fBoundingRadius = val; }
	bool			bTag;
protected:
	void			ParseElelement(const TiXmlElement* componentElement);

	SGameObject*	m_pOwner;

	Vector2D		m_vPosition;
	//--------------------Get from xml------------------------
	//the length of this object's bounding radius
	float	    m_fBoundingRadius;
private:
};

#endif