#pragma once

#include "MessageHandlerBase.h"
#include "MemberFunctionHandler.h"
#include "TypeInfo.h"

struct Event;

class MessageDispatcher
  {
  private:
    typedef std::unordered_map<TypeInfoRef, std::unique_ptr<MessageHandlerBase>, Hasher, EqualTo> Handlers;
    Handlers m_handlers;

  public:
    MessageDispatcher();
    ~MessageDispatcher();

    template < class HandlerType, class EventType >
    void RegisterHandler(HandlerType& i_instance, void (HandlerType::*member_function)(const EventType&));

    void HandleMessage(const Event& i_event);

  };

//////////////////////////////////////////////////////////////////////////

template < typename HandlerType, typename EventType >
void MessageDispatcher::RegisterHandler(HandlerType& i_instance, void (HandlerType::*member_function)(const EventType&))
  {
  m_handlers[typeid(EventType)].reset(new MemberFunctionHandler< HandlerType, EventType >(i_instance, member_function));
  }