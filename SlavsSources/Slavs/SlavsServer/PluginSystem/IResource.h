#ifndef __IRESOURCE_H__
#define __IRESOURCE_H__

#include "Types.h"

#include <Game/IComponent.h>

namespace Slavs
	{

	class IResource : public IComponent
		{
		protected:
			int			m_resource_type;
			size_t	m_initial_resources;
			size_t	m_current_resources;

		public:
			IResource(IGameObject* ip_owner, int i_component_id)
				: IComponent(ip_owner, i_component_id)
				, m_resource_type(-1)
				, m_initial_resources(0)
				, m_current_resources(0)
				{}
			virtual ~IResource(){}

			int GetResourceType() const { return m_resource_type; }
			size_t GetInitialResourceNumber() const { return m_initial_resources; }
			size_t GetCurrentResourceNumber() const { return m_current_resources; }
		};

	} // Slavs

#endif