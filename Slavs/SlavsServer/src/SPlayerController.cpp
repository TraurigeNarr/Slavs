#include "SPlayerController.h"

#include "SGameContext.h"
#include "SGameObject.h"
#include "misc/ServerEnums.h"

#include "Game/GameContext.h"
#include "Game/GameObject.h"
//common
#include <Game/CommandData.h>
#include <Game/Enumerations.h>
#include <Math/Vector2D.h>
#include <Utilities/TemplateFunctions.h>
//standard
#include <algorithm>
#include <cassert>

SPlayerController::SPlayerController(int iMask, Slavs::GameContext& i_context)
	: IController(iMask, i_context)
{}

SPlayerController::~SPlayerController()
{}

void SPlayerController::Init()
{
}

void SPlayerController::TickPerformed()
{
	IController::ControllerStaff();
}

void SPlayerController::HoldPacket(net::Connection* connection, unsigned char* packet, int bytes_read)
{
	Network::PacketType pType = (Network::PacketType)FromChar<int>((char*)packet);
	packet += sizeof(pType);
	bytes_read -= sizeof(pType);

	CommandData* cData = NULL;

	switch(pType)
	{
	case Network::PacketType::PT_Command:
		cData = new CommandData();
		cData->Deserialize((char*)packet);
		HoldCommand(cData);
		break;
	case Network::PacketType::PT_Selection:
		SelectObjects(packet, bytes_read);
		break;
	}
	if(NULL != cData)
		delete cData;
}

void SPlayerController::HoldCommand(CommandData* cData)
{	
	if(cData->HasParameter(CDParams_CreateBuilding))
	{
		int oType = IGameObject::GetGameObjectType(cData->GetCommand());
		if(OT_None != oType && CreateObject(oType))
			m_pContext->AddObject(oType, *static_cast<Vector2D*>(cData->GetExtraData()), this);
	}
	if(cData->HasParameter(CDParams_CharAction))
	{
		printf("Begin processing command for %d objects\n", m_vSelectedObjects.size());
		std::for_each(m_vSelectedObjects.begin(), m_vSelectedObjects.end(), [&cData](SGameObject* selectedObject)
		{
			selectedObject->ProcessCommand(*cData);
		});
	}
}

void SPlayerController::SelectObjects(unsigned char* packet, int bytes_read)
{
	//clear selection
	while(!m_vSelectedObjects.empty())
	{
		IGameObject *item = m_vSelectedObjects.back();
		item->Deselect();
		m_vSelectedObjects.pop_back();
	}

	//deserialize selected objects id and selects them
	for(int i = 0; i < bytes_read; i+=sizeof(long))
	{
		m_vSelectedObjects.push_back(m_pContext->GetGameObject(FromChar<long>((char*)packet)));

		assert(NULL != m_vSelectedObjects.back() && 
			"SPlayerController::SelectObjects: no objects with id");
		
		m_vSelectedObjects.back()->Select();

		packet += sizeof(long);
	}
}