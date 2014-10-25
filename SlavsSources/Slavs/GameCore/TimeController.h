#pragma once

class TickListener
  {
  public:
    virtual void TickPerformed() = 0;
  };

class TimeController
  {
  private:
    //list of listeners that will handle message that tick is performed
    std::vector<TickListener*> m_listeners;

    long m_ms_for_tick;
    //determines milliseconds that passed from last tick
    long m_ms_past;

  public:
    TimeController();
    //@param msForTick - number of milliseconds for one game tick
    TimeController(long i_ms_for_tick);
    ~TimeController();

    void Update(long i_elapsed_time);
    void AddSubscriber(TickListener* ip_listener);
    void RemoveSubscriber(TickListener* ip_listener);
  };