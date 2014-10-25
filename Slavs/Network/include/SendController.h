#pragma once

#include "NetworkAPI.h"
#include "ISendable.h"

#include <vector>
#include <memory>

namespace net
{
  class NETWORKEXPORT SendController
  {
  public:
    std::vector<TSender> m_senders;
  public:
    SendController();
    ~SendController();
    void Update(long i_elapsed_time);

    void RegisterSender(TSender ip_sender);
    void UnregisterSender(TSender ip_sender);
  };
}