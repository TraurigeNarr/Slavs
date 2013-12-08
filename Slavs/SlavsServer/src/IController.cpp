#include "IController.h"

#include "SGameContext.h"
#include "Management/Goverment.h"

#include <cassert>

#include "Management/EconomyManagerBase.h"

IController::IController(int iMask, SGameContext* context)
	: m_pContext(context), m_iMask(iMask)
{
	m_pResourceManager = new SResourceManager();
  mp_goverment.reset(new Goverment(this, std::make_shared<EconomyManagerBase>(), nullptr, nullptr, nullptr, nullptr));
  mp_goverment->GetEconomyManager()->Initialize();
}

IController::~IController()
{}

void IController::ControllerStaff()
{
	m_pResourceManager->AllObjectsUpdated();
}

void IController::SetContext(SGameContext *context)
{
	if(NULL != context)
		m_pContext = context;
}

Goverment& IController::GetGoverment()
{
   return *mp_goverment.get();
}

/*
------------------------------------------------------------------------------
							Serialize/Deserialize
------------------------------------------------------------------------------
*/

int IController::Serialize(char* buf_end, int size) const
{
	if(m_iNeededSize > size)
	{
		assert("<SResourceManager::Serialize>:Not enough size.");
		return 0;
	}

	return m_pResourceManager->Serialize(buf_end, size);
}

int IController::NeededSize() const
{
	m_iNeededSize = m_pResourceManager->NeededSize();
	return m_iNeededSize;
}

bool IController::ChechResources(ObjectType oType)
{
	bool c = m_pResourceManager->CheckAndPickResources(m_pContext->GetNeededResources(oType));
	return c;
}