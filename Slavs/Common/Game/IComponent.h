#ifndef IComponent_h
#define IComponent_h

#include "..\SlavsCommonAPI.h"

#include "../Patterns/Telegram.h"

class GameObjectState;
class TiXmlElement;
class IGameObject;

class /*COMMON_EXPORT*/ IComponent
{
public:
					    IComponent(){}
	virtual			~IComponent(){}

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