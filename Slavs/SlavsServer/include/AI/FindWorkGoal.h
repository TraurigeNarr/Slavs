#ifndef FindWorkGoal_h
#define FindWorkGoal_h

#include "AI/Goal.h"
#include "SHumanComponent.h"
#include "ManufactureRequest.h"

class FindWorkGoal : public Goal<SHumanComponent>
{
public:
					FindWorkGoal(SHumanComponent* pOwner);
					~FindWorkGoal();

	void			Activate();
	int				Process();
	void			Terminate();

	bool			HandleMessage(const Telegram& msg);
protected:
	inline void		ParseRequests(ManufactureRequests& requests);

	//human prefer interesting job with coefficient
	float			m_fInterestingJobCoef;
  float     m_fLookForPaymentCoef;
};

#endif