#include "CPlayerController.h"
//client
#include "OgreFramework.h"
#include "ObjectInformation.h"
#include "ClientEnums.h"
#include "ScreenManager.h"
//game states
#include "IdleCGameState.h"
#include "PlaceBuildingCGameState.h"
#include "GetTargetCGameState.h"
//standard
#include <stdlib.h>

CPlayerController::CPlayerController(Application* owner)
	: m_pOwner(owner)
{
}

CPlayerController::~CPlayerController()
{

}

int CPlayerController::Deserialize(char* buf, int size)
{
	assert(buf && size > 0 &&
		"<CPlayerController::Deserialize>: buffer is NULL");

	GeneralResourceInfo *container = new GeneralResourceInfo(GR_None, 0, 0);

	int i = 0;
	while(i < size)
	{
		i += container->Deserialize(buf + i);
		if(container->m_res_type == GR_None)
			continue;
		ApplyChanges(container);
	}

	delete container;
	
	return i;
}

void CPlayerController::ApplyChanges(GeneralResourceInfo* container)
{
	bool find = false;
	for(unsigned int i = 0; i < m_vResourceContainers.size(); ++i)
	{
		if(m_vResourceContainers[i]->m_res_type == container->m_res_type)
		{
			m_vResourceContainers[i]->uiMaxResources = container->uiMaxResources;
			m_vResourceContainers[i]->uiCurrentResources = container->uiCurrentResources;
			break;
		}
	}
	//if container with such resource type was not found than create it
	if(!find)
		m_vResourceContainers.push_back(new GeneralResourceInfo(container->m_res_type, container->uiCurrentResources, container->uiMaxResources));
	//send changes to Screen system
	ResourceInformation* resInfo = new ResourceInformation();
	resInfo->grGroup = GameResourceBox::DefineResGroup(container->m_res_type);
	resInfo->grType = container->m_res_type;

	resInfo->sValue += StringUtilities::t_to_string(container->uiCurrentResources);
	resInfo->sValue += "/";
	resInfo->sValue += StringUtilities::t_to_string(container->uiMaxResources);
	
	Singleton<ScreenManager>::GetInstancePtr()->GetCurrentScreen()->HandleData(Cmd_UI_ResourcesChanged, resInfo);
}