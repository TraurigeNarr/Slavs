#ifndef MessageListener_h
#define MessageListener_h

#include "MessageSystem/Telegram.h"


class MessageListener
{
public:
	virtual bool HandleMessage(const Telegram& msg) = 0;
	virtual long GetID() const = 0;
};

#endif