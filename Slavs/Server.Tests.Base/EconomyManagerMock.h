#pragma once

#include <SlavsServer/include/Management/IEconomyManager.h>

class MockIEconomyManager : public IEconomyManager {
public:
  MOCK_METHOD1(RegisterEmployee,
    void(Slavs::TGoldKeeper ip_payer));
  MOCK_METHOD1(RemoveEmployee,
    void(Slavs::TGoldKeeper ip_payer));
  MOCK_METHOD1(RegisterEmployer,
    void(Slavs::TGoldKeeper ip_payer));
  MOCK_METHOD1(RemoveEmployer,
    void(Slavs::TGoldKeeper ip_payer));
  MOCK_METHOD1(GetRegisteredPayers,
    void(Slavs::TGoldKeepers& o_payers));
  MOCK_METHOD1(GetEmplyees,
    void(Slavs::TGoldKeepers& o_payers));
  MOCK_METHOD1(GetEmployers,
    void(Slavs::TGoldKeepers& o_payers));
  MOCK_CONST_METHOD1(GetAvailableEmployers,
    void(Slavs::TEmployersInformation& o_available));
  MOCK_METHOD0(Initialize,
    void());
  MOCK_METHOD1(Update,
    void(long i_elapsed_time));
  MOCK_METHOD2(ProcessEvent,
    void(EconomyEvent, void));
  MOCK_METHOD0(GetStoreSystem,
    Slavs::TStoreSystem());
  };
