#pragma once

#include <SlavsServer/PluginSystem/IHuman.h>

namespace Slavs
  {

  class MockIHuman : public IHuman 
    {
    public:
      MockIHuman(Slavs::TGameObject ih_owner, int i_component_id)
        : IHuman(ih_owner, i_component_id)
        {        }
      MOCK_METHOD1(HandleMessage, 
        bool(const Telegram& i_message));
      MOCK_METHOD1(SetHome,
        void(Slavs::HousePtr ip_home));
      MOCK_CONST_METHOD0(GetHome,
        Slavs::HousePtr());
      MOCK_CONST_METHOD0(HasWork,
        bool());
    };

  } // namespace