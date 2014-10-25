#ifndef IComponent_h
#define IComponent_h

#include "..\SlavsCommonAPI.h"

#include "../Patterns/Telegram.h"

class GameObjectState;
class TiXmlElement;
class IGameObject;

class /*COMMON_EXPORT*/ IComponent
{
protected:
  IGameObject*  mp_owner;
  int           m_component_id;
public:
					    IComponent() : m_component_id(-1), mp_owner(nullptr) {}
              IComponent(IGameObject* ip_owner, int i_component_id) : mp_owner(ip_owner), m_component_id(i_component_id) {}
	virtual			~IComponent(){}

  IGameObject*  GetOwner() { return mp_owner; }

  int           GetComponentID() const { return m_component_id; }

	virtual void	TickPerformed(){}
	virtual bool	HandleMessage(const Telegram& msg) = 0;

	virtual void	GetState(GameObjectState&) const{}
	virtual void	ApplyState(const GameObjectState&){}
	/* Check the dependency on other components, can cache the reference on dependency. */
	virtual bool	Probe() { return true; }
protected:
	virtual void	ParseElelement(const TiXmlElement* componentElement){}
};

#endif