#pragma once

struct Event;

class MessageHandlerBase
  {
  private:
    virtual void HandleMessage(const Event& i_event) = 0;

  public:
    virtual ~MessageHandlerBase();
    void ExecuteHandler(const Event& i_event);
  };