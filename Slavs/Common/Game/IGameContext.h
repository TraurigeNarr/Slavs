#ifndef IGameContext_h
#define IGameContext_h

#include "Enumerations.h"

#include "Game/GameObjectState.h"
#include <algorithm>

#include <map>
#include <string>

typedef std::map<long, GameObjectState*>	ObjectStateMap;

template<typename T>
class IGameContext
{
public:
	//typedef std::map<long, T*>		ObjectsMap;
	IGameContext(const std::string& mapName)
		: m_sMapName(mapName)
	{}
	virtual ~IGameContext(){}

	virtual void	Init() = 0;
	virtual void	TickPerformed(){}
  //returns the max needed size
	void			GetAllGameObjectsState(ObjectStateMap& container) const;
	T*				GetGameObject(long id) const;
	unsigned int	GetObjectsCount() const { return m_mGameObjects.size(); }
	std::string		GetMapName() const { return m_sMapName; }
protected:
	void ReleaseGameObjects();
	std::map<long, T*> m_mGameObjects;
	std::string m_sMapName;
};

template<typename T>
void IGameContext<T>::GetAllGameObjectsState(ObjectStateMap& container) const
{
	std::for_each(m_mGameObjects.begin(), m_mGameObjects.end(), [&container](std::pair<long, T*> p)
	{
		if(p.second->HasChanges())
			container.insert(std::pair<long, GameObjectState*>(p.first, p.second->GetState()));
	});
}

template<typename T>
T* IGameContext<T>::GetGameObject(long id) const
{
	std::map<long, T*>::const_iterator iter =  m_mGameObjects.find(id);
	if(m_mGameObjects.end() != iter)
	{
		return (T*)iter->second;
	}
	return NULL;
}

template<typename T>
void IGameContext<T>::ReleaseGameObjects()
{
	std::for_each(m_mGameObjects.begin(), m_mGameObjects.end(), [](std::pair<long, T*> p)
	{
		delete p.second;
		p.second = NULL;
	});
	m_mGameObjects.clear();
}

#endif