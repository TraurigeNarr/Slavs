#ifndef CGameContext_h
#define CGameContext_h
#include "CGameObject.h"

#include <Game/Enumerations.h>
#include <Game/IGameContext.h>
#include <Game/TimeController.h>

#include <OgreSceneQuery.h>

class GameObjectState;
class CTerrain;
class TiXmlElement;

typedef std::map<long, CGameObject*>		ObjectsMap;

class CGameContext : public IGameContext<CGameObject>, public TickListener
{
public:
	CGameContext(const std::string& mapName, int ownMask);
	~CGameContext();

	void				Init();
	void				TickPerformed();
  virtual void            ReleaseContext() override;

	//each update applies state from server
	void				ApplyState(long id, const GameObjectState& state);
	void				RemoveObject(CGameObject* gameObject);

	//special methods for adding terrain and camera
	void				AddTerrain(const std::string &);

	//getters
	CTerrain*			GetTerrain() const { return m_pTerrain; }
	const TiXmlElement* GetConfigElement(int oType) const;
	IGameObject*		GetObjectByRay(const Ogre::RaySceneQueryResult &results) const;

	ObjectsMap::const_iterator GetIteratorBegin() const { return m_mGameObjects.begin(); }
	ObjectsMap::const_iterator GetIteratorEnd() const { return m_mGameObjects.end(); }

	void				InitObjectsMap();
protected:
	void				ClearData();

	std::map<int, const TiXmlElement*> m_mConfigElements;

	CTerrain *m_pTerrain;

	int m_iOwnMask;

};

#endif