#include "ISendable.h"

#include "SendController.h"

using namespace net;

ISendable::ISendable(SendController* ip_controller, long i_wait_time)
  : mp_controller(ip_controller)
  , m_wait_time(i_wait_time)
  , m_current_time(0)
{
  mp_controller->RegisterSender(std::shared_ptr<ISendable>(this));
}

ISendable::~ISendable()
{
  mp_controller->UnregisterSender(std::shared_ptr<ISendable>(this));
}

void ISendable::Update(long i_elapsed_time)
{
  m_current_time += i_elapsed_time;
}

bool ISendable::HasChanges() const
{
  return m_current_time >= m_wait_time;
}

size_t ISendable::Serialize(Packet& io_packet) const
{
  int result = _Serialize(io_packet);
  m_current_time = 0;
  return result;
}

size_t ISendable::NeededSize() const
{
  return _NeededSize();
}

bool ISendable::SetController(SendController* ip_controller)
{
  if(nullptr != mp_controller)
    return false;

  mp_controller = ip_controller;
  return true;
}