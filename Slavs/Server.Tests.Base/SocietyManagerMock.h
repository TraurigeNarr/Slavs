#pragma once

#include <SlavsServer/include/Management/ISocietyManager.h>

class MockISocietyManager : public ISocietyManager 
  {
  public:
    MOCK_METHOD0(PopulationIncrease,
      void());
    MOCK_METHOD0(PopulationDecrease,
      void());
    MOCK_METHOD1(RegisterHouse,
      void(Slavs::HousePtr ip_house));
    MOCK_METHOD1(UnregisterHouse,
      void(Slavs::HousePtr ip_house));
  };
