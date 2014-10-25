#ifndef FindWorkGoal_h
#define FindWorkGoal_h

#include "AI/Goal.h"
#include "ManufactureRequest.h"
#include "SHumanComponent.h"

#include "Types.h"

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
	void		ParseRequests(const Slavs::TEmployersInformation& i_informations);

	//human prefer interesting job with coefficient
	float			m_fInterestingJobCoef;
  float     m_fLookForPaymentCoef;
};

#endif