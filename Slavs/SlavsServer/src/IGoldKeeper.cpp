#include "IGoldKeeper.h"

IGoldKeeper::IGoldKeeper(size_t i_start_money /* = 0 */)
  : m_money(i_start_money)
{

}

IGoldKeeper::~IGoldKeeper()
{
  m_callback.clear();
}

void IGoldKeeper::GiveMoney(size_t i_number)
{
  m_money += i_number;

  if(!m_callback.empty())
    m_callback(m_money);
}

bool IGoldKeeper::GetMoney(size_t i_number)
{
  if(m_money < i_number)
    return false;

  m_money -= i_number;
  
  if(!m_callback.empty())
    m_callback(m_money);

  return true;
}

void IGoldKeeper::SetCallback(boost::function<void(size_t)> i_function_callback)
{
  m_callback = i_function_callback;
}