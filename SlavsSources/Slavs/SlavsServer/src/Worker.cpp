#include "Worker.h"
#include "HumanStates.h"
#include "misc\ServerEnums.h"
//AI
#include "AI/Goal_Think.h"
//components
#include "SGameObject.h"
#include "SHumanComponent.h"
#include "SManufacureCom.h"
//misc
#include <Patterns\Singleton.h>
#include <Utilities\TemplateFunctions.h>
//standard
#include <cassert>
#include <cmath>

const int MAX_NUM_OF_GROWINGS = 16;

Worker::Worker(SHumanComponent* human)
	: Profession(human), m_pWork(nullptr)
{
}

Worker::~Worker()
{
	if(NULL != m_pWork)
	{
		m_pWork->HandleMessage(Telegram(0,0,0, Msg_HumanDied, m_pHuman));
	}
}

void Worker::TickPerformed()
{
  assert(m_pWork != nullptr);
}

bool Worker::HandleMessage(const Telegram& msg)
{
	bool processed = false;
	switch(msg.Msg)
	{
	case Msg_PeriodEnds:
		{
			m_pWork->HandleMessage(Telegram(0,0,0, Msg_WorkerPassResources, &m_Bag.iBagCapacity));
			m_Bag.iCurrentResource = 0;
			processed = true;
      //check for growing
      if(MAX_NUM_OF_GROWINGS >= m_growing_nums)
      {
        ++m_current_tick_for_growth;
        //TODO: look throw algorithm of learning curve
        if(m_current_tick_for_growth >= m_pWork->GetGrowthPeriod()<<m_growing_nums)
          IncreaseSkill();
      }
			break;
		}
	case Msg_HumanFired:
		{
			m_pHuman->GetFSM()->ChangeState(Singleton<HumanIdle>::GetInstancePtr());
			m_pWork = NULL;
			processed = true;
			break;
		}
	}
	return processed;
}

bool Worker::IsPlaceOfWork(void *placeOfWork) const
{
	return m_pWork == placeOfWork;
}

void Worker::SetPlaceOfWork(SManufacureCom* work)
{
	m_pWork = work;
  m_has_changes = true;
	if(0 == m_fSkill)
  {
    m_fSkill = m_pWork->GetInitialSkill();
    m_ProfessionType = m_pWork->GetWorkerType();
    m_current_growing_skill = m_pWork->GetSkillGrowth();
    m_growing_nums = 1;
    m_Bag.iInitialCapacity = work->GetIniResMines();

    if(!m_callback.empty())
      m_callback(m_fSkill);
  }
	m_Bag.iBagCapacity = Round<int>(m_Bag.iInitialCapacity*m_fSkill);
	m_Bag.iCurrentResource = 0;
}

void Worker::IncreaseSkill()
{
  //TODO save skill in history
  ++m_growing_nums;

  m_fSkill += m_current_growing_skill;
  m_current_growing_skill *= (1-m_pWork->GetLearningCoefficient());
  m_current_tick_for_growth = 0;
  
  m_Bag.iBagCapacity = Round<int>(m_Bag.iInitialCapacity*m_fSkill);

  m_has_changes = true;

  if(!m_callback.empty())
    m_callback(m_fSkill);

	m_pHuman->GetOwner()->StateChanged();
}

size_t Worker::GetRelativeSkill() const
{
  float skill_rank_segment = m_pWork->GetMaxSkill() - m_pWork->GetInitialSkill();

  size_t skill_rank = 0;

  for(int i = 0; i < 4; ++i)
    if((m_pWork->GetInitialSkill()+(i+1)*skill_rank_segment) <= m_fSkill)
      ++skill_rank;
    else
      return skill_rank;
  return skill_rank;
}