#pragma once

#include <SlavsServer/include/Management/IStoreSystem.h>

class MockIStoreSystem : public IStoreSystem {
public:

  bool m_flush_resources;

  MockIStoreSystem (IEconomyManager& i_owner, bool i_flush_resources = true)
    : IStoreSystem(i_owner)
    , m_flush_resources(i_flush_resources)
    {    }

  virtual void Add(Slavs::ResourcePair& io_resource) override
    {
    AddMock(io_resource);
    if (m_flush_resources)
      io_resource.second = 0;
    }

  MOCK_METHOD1(Register,
    void(Slavs::StorePtr ip_store_house));
  MOCK_METHOD1(Remove,
    void(Slavs::StorePtr ip_store_house));
  MOCK_METHOD1(UpdateStoreInformation,
    void(Slavs::StorePtr ip_store_house));
  MOCK_METHOD1(Add,
    void(Slavs::TResources& io_new_resources));
  MOCK_METHOD1(Add,
    void(Slavs::TGameResourceBox iop_new_resource));
  MOCK_METHOD1(AddMock,
    void(Slavs::ResourcePair& io_resource));
  MOCK_METHOD1(Add,
    void(Slavs::ResourcesCountSet& i_resources));
  MOCK_METHOD2(ProcessEvent,
    void(EconomyEvent, void*));
  MOCK_METHOD2(PeekResources,
    bool(GameResourceType i_type, size_t i_number));
  MOCK_METHOD2(GetResources,
    size_t(GameResourceType i_type, size_t i_number));
  MOCK_METHOD1(GetResource,
    TResourcePair(GameResourceType i_type));
  MOCK_METHOD0(GetStores,
    Slavs::Stores&());
  MOCK_METHOD1(GetResourceInformation,
    GeneralResourceInfo&(GameResourceType i_resource_type));
  };
