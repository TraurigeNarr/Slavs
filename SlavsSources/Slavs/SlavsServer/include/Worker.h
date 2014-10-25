#ifndef Worker_h
#define Worker_h

#include "Profession.h"
#include "SManufacureCom.h"

class SManufacureCom;

class Worker : public Profession
{
	struct WorkersBag
	{
		int iInitialCapacity;
		int iBagCapacity;
		int iCurrentResource;

		bool PutInBag(int val)
		{
			if(iBagCapacity - iCurrentResource >= val)
			{
				iCurrentResource += val;
				return true;
			}
			return false;
		}
	};
public:
					Worker(SHumanComponent* human);
					~Worker();

	void			TickPerformed();
	bool			HandleMessage(const Telegram& msg);
	bool			IsPlaceOfWork(void *placeOfWork) const;
	void			SetPlaceOfWork(SManufacureCom* work);
  virtual size_t GetRelativeSkill() const override;
protected:
  void			IncreaseSkill();

	WorkersBag		m_Bag;
	SManufacureCom*	m_pWork;
};

#endif