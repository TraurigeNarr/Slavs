#ifndef Telegram_h
#define Telegram_h

#include <math.h>

class Telegram
{
public:
	/*Object, who sent this telegram*/
	long Sender;
	/*Object that is to receive this telegram*/
	long Receiver;
	/*Message itself*/
	int Msg;
	/*
		Messages can be dispatched immediately or delayed for a specific amount
		of time. If a delay is necessary this field is stamped with the time the
		message should be dispatched.
	*/
	double DispatchTime;

	/*Any additional information.*/
	void* ExtraInfo;

	Telegram():DispatchTime(-1),
		Sender(-1),
		Receiver(-1),
		Msg(-1)
	{}


	Telegram(double time,
		int    sender,
		int    receiver,
		int    msg,
		void*  info = 0): DispatchTime(time),
		Sender(sender),
		Receiver(receiver),
		Msg(msg),
		ExtraInfo(info)
	{}
};

//these telegrams will be stored in a priority queue. Therefore the >
//operator needs to be overloaded so that the PQ can sort the telegrams
//by time priority. Note how the times must be smaller than
//SmallestDelay apart before two Telegrams are considered unique.
const double SmallestDelay = 0.25;


inline bool operator==(const Telegram& t1, const Telegram& t2)
{
	return ( fabs(t1.DispatchTime-t2.DispatchTime) < SmallestDelay) &&
		(t1.Sender == t2.Sender)        &&
		(t1.Receiver == t2.Receiver)    &&
		(t1.Msg == t2.Msg);
}

inline bool operator<(const Telegram& t1, const Telegram& t2)
{
	if (t1 == t2)
	{
		return false;
	}

	else
	{
		return  (t1.DispatchTime < t2.DispatchTime);
	}
}

//handy helper function for dereferencing the ExtraInfo field of the Telegram 
//to the required type.
template <class T>
inline T DereferenceToType(void* p)
{
	return *(T*)(p);
}

#endif