#pragma once

#include <SlavsServer/include/Management/IStoreSystem.h>

class MockIStoreSystem : public IStoreSystem {
public:
  MOCK_METHOD1(Register,
    void(Slavs::StorePtr ip_store_house));
  MOCK_METHOD1(Remove,
    void(Slavs::StorePtr ip_store_house));
  MOCK_METHOD1(Add,
    void(Slavs::TResources& i_new_resources));
  MOCK_METHOD1(Add,
    void(Slavs::TGameResourceBox i_new_resource));
  MOCK_METHOD1(Add,
    void(Slavs::ResourcePair i_resource));
  MOCK_METHOD1(Add,
    void(Slavs::ResourcesCountSet i_resources));
  MOCK_METHOD2(ProcessEvent,
    void(EconomyEvent, void));
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
