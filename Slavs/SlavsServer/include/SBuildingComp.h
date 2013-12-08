#ifndef SBuildingComp_h
#define SBuildingComp_h

#include "Game/IComponent.h"

class Telegram;
class SGameObject;
class TiXmlElement;

class SBuildingComp : public IComponent
{
public:
	SBuildingComp(SGameObject* owner, const TiXmlElement* componentElement);
	~SBuildingComp();

	bool			HandleMessage(const Telegram& msg);

	SGameObject*	GetOwner() const { return m_pOwner; }
	void			GetState(GameObjectState&) const;

	int				GetMaxHP() const { return m_iHP; }
	int				GetCurrentHP() const { return m_iCurrentHP;}
	int				GetArmor() const { return m_iArmor; }
protected:
	void			ParseElelement(const TiXmlElement* componentElement);

	SGameObject*	m_pOwner;

	int				m_iHP;
	int				m_iCurrentHP;
	int				m_iArmor;
};

#endif