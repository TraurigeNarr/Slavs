#pragma once

#include "IController.h"
#include "IControllerPlugin.h"

#include <vector>

namespace net
{
  class Connection;
}

class AIController : public IController
{
private:
  std::vector<IControllerPlugin> m_plugins;
public:
  virtual void TickPerformed() override;

  virtual void Init() override;
  virtual void HoldPacket(net::Connection* connection, unsigned char* packet, int bytes_read) override;
};