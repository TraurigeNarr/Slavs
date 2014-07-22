#pragma once

#include <SlavsServer/include/IController.h>

class MockIController : public IController 
  {
  public:
    MockIController(int iMask, Slavs::GameContext& i_context)
      : IController(iMask, i_context)
      {      }
    MOCK_METHOD0(TickPerformed,
      void());
    MOCK_METHOD0(Init,
      void());
    MOCK_METHOD3(HoldPacket,
      void(net::Connection* connection, unsigned char* packet, int bytes_read));
    MOCK_METHOD1(NotifyObjectChanges,
      void(const std::map<long, GameObjectState*>& ip_object_states));
  };
