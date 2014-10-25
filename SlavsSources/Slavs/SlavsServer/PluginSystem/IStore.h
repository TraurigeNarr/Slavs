#pragma once

#include <Game/IComponent.h>

#include <memory>
#include <vector>

class GameResourceBox;
class GameResourceContainer;

namespace Slavs
  {

  class IStore : public IComponent
    {
    public:
      IStore(IGameObject* ip_owner, int i_component_id) : IComponent(ip_owner, i_component_id){}
      virtual ~IStore(){}

      /// <summary>
      /// Checks if this resource is available for this store and if so than
      /// place it to the container. In resource box decreases number of resources.
      /// </summary>
      virtual bool AddResource (GameResourceBox& io_resource_box) = 0;
      
      /// <summary>AddResource
      /// <para><see cref="Slavs::IStore::AddResource"/> for information about output statements.</para>
      /// if there is not enough place in store returns false
      /// </summary>
      virtual bool AddResource (int i_type, size_t i_number) = 0;

      /// <summary>AddResource
      /// <para><see cref="Slavs::IStore::AddResource"/> for information about output statements.</para>
      /// If there is not enough place in store it adds as much as it can take and returns number of resources that was added
      /// </summary>
      virtual size_t AddResourceForcely (int i_type, size_t i_number) = 0;

      /// <summary>
      /// Returns number of resources that were in store;
      ///  If in store enough resources returned number equals i_number
      /// </summary>
      /// <param name='i_type '>type of resources that is required</param>
      /// <param name='i_number'>number of resources</param>
      virtual size_t PeekResource (int i_type, size_t i_number) const = 0;

      /// <summary>
      /// Returns true if store can accept one more resource container
      ///  This can be false for sophisticated economy models where maximum number depends on workers
      /// </summary>
      /// <param name='ip_container'> container will be added to store
      virtual bool AddResourceContainer(std::shared_ptr<GameResourceContainer> ip_container) = 0;

      /// <summary>
      /// Returns number of resources that were picked; 0 if there is not enough resources
      /// </summary>
      /// <param name='i_type '>type of resources that is required</param>
      /// <param name='i_number'>number of resources</param>
      virtual size_t GetResource (int i_type, size_t i_number) = 0;

      /// <summary>
      /// Returns number of resources that were picked (all resources of required type if 
      /// number of resources in store <= number of required
      /// </summary>
      /// <param name='i_type '>type of resources that is required</param>
      /// <param name='i_number'>number of resources</param>
      virtual size_t GetResourceForcely (int i_type, size_t i_number) = 0;

      /// <summary>
      /// Returns resources that are held in this store
      /// </summary>
      virtual const std::vector<std::shared_ptr<GameResourceContainer>>& GetResources() const = 0;
    };

  } // Slavs