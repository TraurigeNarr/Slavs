#pragma once

#include "SlavsServerAPI.h"

#include "Management/EventsEnumerations.h"

#include <boost\function.hpp>

class IGoldKeeper
  {
  private:
    size_t m_money;
    boost::function<void(size_t)> m_callback;

  public:
    SLAVS_SERVER_EXPORT         IGoldKeeper(size_t i_start_money = 0);
    virtual SLAVS_SERVER_EXPORT ~IGoldKeeper();

    void SLAVS_SERVER_EXPORT    GiveMoney(size_t i_number);
    bool SLAVS_SERVER_EXPORT    GetMoney(size_t i_number);
    size_t HasMoney() const { return m_money; }

    void SLAVS_SERVER_EXPORT    SetCallback(boost::function<void(size_t)>);

    virtual void ProcessEconomyEvent(EconomyEvent i_event, void* ip_data = nullptr) = 0;
  };