#ifndef MessageDispatcher_h
#define MessageDispatcher_h

#include "MessageSystem\Telegram.h"
#include "MessageSystem\MessageListener.h"

#include <set>
#include <time.h>

#include "Singleton.h"

//to make code easier to read
const double SEND_MSG_IMMEDIATELY = 0.0f;
const int   NO_ADDITIONAL_INFO   = 0;

//to make life easier...
#define Dispatcher MessageDispatcher::GetSingletonPtr()

template < class T>
class MessageDispatcher : public Singleton<MessageDispatcher<T>>
{
private:  

	//a std::set is used as the container for the delayed messages
	//because of the benefit of automatic sorting and avoidance
	//of duplicates. Messages are sorted by their dispatch time.
	std::set<Telegram> m_sPriorityQ;

	//needed for getting objects by ID
	T *m_pGameContext;
	//this method is utilized by DispatchMessage or DispatchDelayedMessages.
	//This method calls the message handling member function of the receiving
	//entity, pReceiver, with the newly created telegram
	void Discharge(MessageListener* pReceiver, const Telegram& msg);

public:
	MessageDispatcher(T* context) 
		: m_pGameContext(context)
	{}
	~MessageDispatcher(){}

	//send a message to another agent. Receiving agent is referenced by ID.
	void DispatchMsg(double  delay,
		long    sender,
		long    receiver,
		int		msg,
		void*	ExtraInfo);

	//send out any delayed messages. This method is called each time through   
	//the main game loop.
	void DispatchDelayedMsgs();
};

template < class T>
void MessageDispatcher<T>::Discharge(MessageListener* pReceiver, const Telegram& msg)
{
	pReceiver->HandleMessage(msg);
}

template < class T>
void MessageDispatcher<T>::DispatchMsg(double delay, long sender, long receiver, int msg, void* ExtraInfo)
{
	MessageListener* pSender   = m_pGameContext->GetGameObject(sender);
	MessageListener* pReceiver = m_pGameContext->GetGameObject(receiver);

	//make sure the receiver is valid
	if (pReceiver == NULL)
		return;

	//create the telegram
	Telegram telegram(0, pSender->GetID(), pReceiver->GetID(), msg, ExtraInfo);

	//if there is no delay, route telegram immediately                       
	if (delay <= 0.0f)                                                        
	{
		/*cout << "\nInstant telegram dispatched at time: " << Clock->GetCurrentTime()
			<< " by " << GetNameOfEntity(pSender->ID()) << " for " << GetNameOfEntity(pReceiver->ID()) 
			<< ". Msg is "<< MsgToStr(msg);*/

		//send the telegram to the recipient
		Discharge(pReceiver, telegram);
	}

	//else calculate the time when the telegram should be dispatched
	else
	{
		double CurrentTime = clock() / CLOCKS_PER_SEC; 

		telegram.DispatchTime = CurrentTime + delay;

		//and put it in the queue
		m_sPriorityQ.insert(telegram);   

		/*cout << "\nDelayed telegram from " << GetNameOfEntity(pSender->ID()) << " recorded at time " 
			<< Clock->GetCurrentTime() << " for " << GetNameOfEntity(pReceiver->ID())
			<< ". Msg is "<< MsgToStr(msg);*/

	}
}

template < class T>
void MessageDispatcher<T>::DispatchDelayedMsgs()
{
	double CurrentTime = clock() / CLOCKS_PER_SEC;

		while( !m_sPriorityQ.empty() &&
			(m_sPriorityQ.begin()->DispatchTime < CurrentTime) && 
			(m_sPriorityQ.begin()->DispatchTime > 0) )
		{
			//read the telegram from the front of the queue
			const Telegram& telegram = *m_sPriorityQ.begin();

			//find the recipient
			MessageListener* pReceiver = m_pGameContext->GetGameObject(telegram.Receiver);

			if(NULL == pReceiver)
				return;

			/*cout << "\nQueued telegram ready for dispatch: Sent to " 
				<< GetNameOfEntity(pReceiver->ID()) << ". Msg is " << MsgToStr(telegram.Msg);*/

			//send the telegram to the recipient
			Discharge(pReceiver, telegram);

			//remove it from the queue
			m_sPriorityQ.erase(m_sPriorityQ.begin());
		}
}


#endif