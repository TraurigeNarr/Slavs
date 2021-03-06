#pragma once

#include "MessageHandlerBase.h"

template < class HandlerType, class EventType >
class MemberFunctionHandler : public MessageHandlerBase
  {
  public:
    typedef void (HandlerType::*MemberFunc)(EventType);

  private:
    HandlerType&  m_instance;
    MemberFunc    m_function;

  public:
    MemberFunctionHandler(HandlerType& i_handler_instance, MemberFunc i_member_func) 
      : m_instance(i_handler_instance)
      , m_function(i_member_func) 
      {        }

    virtual void HandleMessage(const Event& i_event) override
      {
      (m_instance.*m_function)(static_cast< EventType& >(i_event));
      }
  };