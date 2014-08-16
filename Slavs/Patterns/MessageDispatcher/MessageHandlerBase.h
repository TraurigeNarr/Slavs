#pragma once

#include "..\PatternsAPI.h"

struct Event;

class MessageHandlerBase
  {
  private:
    virtual void HandleMessage(const Event& i_event) = 0;

  public:
    virtual PATTERN_EXPORT  ~MessageHandlerBase();
    void PATTERN_EXPORT     ExecuteHandler(const Event& i_event);
  };