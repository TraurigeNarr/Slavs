#ifndef __GAMEWORLD_H__
#define __GAMEWORLD_H__

#include "SlavsServerAPI.h"
#include "Types.h"

#include <boost/noncopyable.hpp>

namespace Slavs
	{

	class IResource;

	/*
	GameWorld class that contains data about world
		1. Terrain
		2. Resources
		3. Tiles
	*/

	class GameWorld : boost::noncopyable
		{
		private:
			std::vector<IResource*> m_resources;

		public:
			SLAVS_SERVER_EXPORT														GameWorld();
			SLAVS_SERVER_EXPORT														~GameWorld();

			const std::vector<IResource*>&								GetResources() const;
			SLAVS_SERVER_EXPORT std::vector<IResource*>		GetResourcesOfType(int i_type) const;

			SLAVS_SERVER_EXPORT void											RegisterResource(IResource* ip_resource);
			SLAVS_SERVER_EXPORT void											UnregisterResource(IResource* ip_resource);
		};

	inline const std::vector<IResource*>& GameWorld::GetResources() const 
		{ return m_resources; }

	} // Slavs

#endif