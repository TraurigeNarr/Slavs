#pragma once

#include "SlavsServerAPI.h"

#include <boost\function.hpp>

class SLAVS_SERVER_EXPORT IGoldKeeper
{
public:
  IGoldKeeper(size_t i_start_money = 0);
  virtual ~IGoldKeeper();

  void GiveMoney(size_t i_number);
  bool GetMoney(size_t i_number);
  size_t HasMoney() const { return m_money; }

  void SetCallback(boost::function<void(size_t)>);
private:
  size_t m_money;
  boost::function<void(size_t)> m_callback;
};