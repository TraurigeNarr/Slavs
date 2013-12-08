#pragma once

#include "NetworkAPI.h"

#include "Packet.h"

#include <memory>

namespace net
{
  class SendController;

  class NETWORKEXPORT ISendable
  {
  friend SendController;
  private:
    mutable long m_current_time;
    long m_wait_time;
    SendController* mp_controller;
  protected:
    mutable size_t m_needed_size;
  public:
    //i_wait_time - waits for that time to send data
    ISendable(SendController* ip_controller, long i_wait_time);
    virtual ~ISendable();

    void            Update(long i_elapsed_time);
    bool            HasChanges() const;
    size_t          Serialize(Packet& io_packet) const;
    size_t          NeededSize() const;
    bool            SetController(SendController* ip_controller);

  protected:
    virtual int     _Serialize(Packet& io_packet) const = 0;
    virtual size_t  _NeededSize() const = 0;
  };

  typedef std::shared_ptr<ISendable> TSender;
}