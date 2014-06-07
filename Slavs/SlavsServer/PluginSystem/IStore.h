#pragma once

#include <memory>
#include <vector>

class GameResourceBox;

namespace Slavs
  {

  class IStore
    {
    public:
      virtual ~IStore(){}

      /// <summary>
      /// Checks if this resource is available for this store and if so than
      /// place it to the container. Returns the number of resource that it add to container.
      /// <para>Here's how you could make a second paragraph in a description. <see cref="System::Console::WriteLine"/> for information about output statements.</para>
      /// </summary>
      virtual bool AddResource (GameResourceBox& i_resource_box) = 0;
      
      /// <summary>AddResource
      /// <para><see cref="Slavs::IStore::AddResource"/> for information about output statements.</para>
      /// </summary>
      virtual void AddResource (int i_type, size_t i_number) = 0;

      /// <summary>
      /// Returns true if number of resources type i_type in store >= i_number
      /// </summary>
      /// <param name='i_type '>type of resources that is required</param>
      /// <param name='i_number'>number of resources</param>
      virtual bool PeekResource (int i_type, size_t i_number) = 0;


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
    };

  } // Slavs