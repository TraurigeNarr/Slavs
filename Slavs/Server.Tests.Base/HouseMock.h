#pragma once

#include <SlavsServer/PluginSystem/IHouse.h>

namespace Slavs
  {
  class MockIHouse : public Slavs::IHouse 
    {
    public:
      MockIHouse(Slavs::TGameObject ih_owner, int i_component_id)
        : IHouse(ih_owner, i_component_id)
        {        }
      MOCK_METHOD1(HandleMessage, 
        bool(const Telegram& i_message));
      MOCK_CONST_METHOD0(GetPopulation,
        size_t());
      MOCK_METHOD1(IncreaseMaximumInhabitatnsBy,
        void(size_t i_number));
      MOCK_CONST_METHOD0(GetUnemployedPopulation,
        size_t());
      MOCK_CONST_METHOD1(GetUnemployedPopulation,
        void(Humans& o_unemployed));
      MOCK_CONST_METHOD0(GetFreePlaces,
        size_t());
      MOCK_METHOD1(AddInhabitant,
        bool(HumanPtr ip_inhabitant));
      MOCK_METHOD1(RemoveInhabitant,
        void(HumanPtr ip_inhabitant));
      MOCK_METHOD0(HumanStateChanged,
        void());
    };
  }  // namespace