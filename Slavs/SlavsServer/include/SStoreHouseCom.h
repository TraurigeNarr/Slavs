#ifndef SStoreHouseCom_h
#define SStoreHouseCom_h

/*
---------------------------------------------------
Component that keep track of specific resource types.
---------------------------------------------------
*/

#include "Game/IComponent.h"
#include "Game/GameResources.h"

#include <vector>

class Telegram;
class SGameObject;
class TiXmlElement;
class SBuildingComp;

class SStoreHouseCom : public IComponent
{
public:
	SStoreHouseCom(SGameObject* owner, const TiXmlElement* componentElement);
	~SStoreHouseCom();

	bool						HandleMessage(const Telegram& msg) { return false; }

	SGameObject*				GetOwner() const { return m_pOwner; }
	void						GetState(GameObjectState&) const;
	//check for building component
	bool						Probe();

	//gets resource if it exists in this store, NULL otherwise
	//std::shared_ptr<GameResourceContainer>		GetResource(GameResourceType grType) const;
	//checks if this resource is available for this store and if so than
	//place it to the container. Returns the number of resource that it add to container.
	bool						AddResource(GameResourceBox* gameResource) const;
	//peek resources from store house and returns the number of resources that were peeked
	size_t						PeekResources(GameResourceType grType, size_t number);
	
	const TResourceContainers& GetResources() const { return m_vResources; }
protected:
	void						ParseElelement(const TiXmlElement* componentElement);

	SGameObject*				m_pOwner;

	//building component
	SBuildingComp*				m_pBuilding;

	TResourceContainers 	m_vResources;
};

#endif