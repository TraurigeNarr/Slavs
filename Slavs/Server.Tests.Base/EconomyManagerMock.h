#pragma once

#include <SlavsServer/include/Management/IEconomyManager.h>

#include "StoreSystemMock.h"

class MockIEconomyManager : public IEconomyManager 
  {
  private:
    std::unique_ptr<IStoreSystem> mp_store_system;
  public:
    MockIEconomyManager()
      : mp_store_system(new MockIStoreSystem(*this))
      {      }
    MOCK_METHOD1(RegisterEmployee,
      void SLAVS_SERVER_EXPORT(Slavs::TGoldKeeper ip_payer));
    MOCK_METHOD1(RemoveEmployee,
      void SLAVS_SERVER_EXPORT(Slavs::TGoldKeeper ip_payer));
    MOCK_METHOD1(RegisterEmployer,
      void SLAVS_SERVER_EXPORT(Slavs::TGoldKeeper ip_payer));
    MOCK_METHOD1(RemoveEmployer,
      void SLAVS_SERVER_EXPORT(Slavs::TGoldKeeper ip_payer));
    MOCK_METHOD1(GetRegisteredPayers,
      void SLAVS_SERVER_EXPORT(Slavs::TGoldKeepers& o_payers));
    MOCK_METHOD1(GetEmplyees,
      void SLAVS_SERVER_EXPORT(Slavs::TGoldKeepers& o_payers));
    MOCK_METHOD1(GetEmployers,
      void SLAVS_SERVER_EXPORT(Slavs::TGoldKeepers& o_payers));
    MOCK_CONST_METHOD1(GetAvailableEmployers,
      void SLAVS_SERVER_EXPORT(Slavs::TEmployersInformation& o_available));
    MOCK_METHOD0(Initialize,
      void());
    MOCK_METHOD1(Update,
      void(long i_elapsed_time));
    MOCK_METHOD2(ProcessEvent,
      void(EconomyEvent, void*));
  
    virtual Slavs::TStoreSystem                           GetStoreSystem() const override
      {
      return mp_store_system.get();
      }
    virtual void                                          SetStoreSystem(std::unique_ptr<IStoreSystem>&& ip_store) override
      {
      throw std::exception("Not use this in test");
      }
    virtual Slavs::ResourceManagerPtr                     GetResourceManager() const override
      {
      return nullptr;
      }
    virtual void                                          SetResourceManager(std::unique_ptr<IResourceManager>&& ip_resource_manager) override
      {
      throw std::exception("Not use this in test");
      }
  };
