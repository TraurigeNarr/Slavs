#include "GameWorld.h"

#include <SlavsServer/PluginSystem/IResource.h>

namespace Slavs
	{

	GameWorld::GameWorld()
		{}

	GameWorld::~GameWorld()
		{}

	std::vector<IResource*>	GameWorld::GetResourcesOfType(int i_type) const
		{
		std::vector<IResource*> resources;

		for (auto p_resource : m_resources)
			{
			if (p_resource->GetResourceType() == i_type)
				resources.push_back(p_resource);
			}

		return resources;
		}

	void	GameWorld::RegisterResource(IResource* ip_resource)
		{
		m_resources.push_back(ip_resource);
		}

	void	GameWorld::UnregisterResource(IResource* ip_resource)
		{
		m_resources.push_back(ip_resource);
		}

	} // Slavs