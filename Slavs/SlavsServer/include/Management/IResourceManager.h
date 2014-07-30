#pragma once

#include "IManager.h"
#include "Types.h"

//////////////////////////////////////////////////////////////////////////

class GameResourceBox;
class GameResourceContainer;

//////////////////////////////////////////////////////////////////////////

class IResourceManager : public IManager
  {
  public:
    virtual ~IResourceManager() {}

    /// <summary>
    /// Add as many resources as it can
    ///   return true if all resources are in stores; false othervise
    /// </summary>
    virtual bool AddResource (GameResourceBox& io_resource_box) = 0;

    /// <summary>AddResource
    /// <para><see cref="Slavs::IStore::AddResource"/> for information about output statements.</para>
    /// if there is not enough place in stores returns false
    /// </summary>
    virtual bool AddResource (int i_type, size_t i_number) = 0;

    /// <summary>
    /// Returns number of resources that were in store;
    ///  If in store enough resources returned number equals i_number
    /// </summary>
    /// <param name='i_type '>type of resources that is required</param>
    /// <param name='i_number'>number of resources</param>
    virtual size_t PeekResource (int i_type, size_t i_number) const = 0;

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
    ///  Gets/sets economy manager to which this resource manager belongs
    /// </summary
    virtual Slavs::TEconomy GetEconomy() const = 0;
    virtual void            SetEconomy(Slavs::TEconomy ip_economy) = 0;
  };