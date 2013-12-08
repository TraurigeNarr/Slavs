#ifndef CGameObject_h
#define CGameObject_h

#include "ObjectInformation.h"

#include <Game/IGameObject.h>

class GameObjectState;
class TiXmlElement;

class CGameObject : public IGameObject
{
public:
	CGameObject(long ID, ObjectType otype, int iMask = 0x00, const TiXmlElement* configElement = NULL /*const std::string& configFile = "", SController* owner = NULL*/);
	virtual ~CGameObject();

	virtual void		Init();
	virtual void		TickPerformed();

	virtual void		ApplyState(const GameObjectState& state);
	ObjectInformation*	GetObjectInfo() const { return m_ObjectInformation; }
protected:
	void				ReCreateObject(ObjectType oType);
	void				ParseElement(const TiXmlElement* configElement);
	inline void			ParseState(const GameObjectState& state);

	ObjectInformation*	m_ObjectInformation;
};

#endif