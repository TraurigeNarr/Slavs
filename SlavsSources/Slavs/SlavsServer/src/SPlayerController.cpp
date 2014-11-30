#include "SPlayerController.h"

#include "SGameContext.h"
#include "SGameObject.h"
#include "misc/ServerEnums.h"

#include "Game/GameContext.h"
#include "Game/GameObject.h"
#include "Main/ServerMain.h"
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
  enum Commands
    {
    CreateManufacture = 0,
    CreateStore = 3
    };
  char* p_packet = reinterpret_cast<char*>(packet);
  Network::PacketType packet_type = (Network::PacketType)FromChar<int>(p_packet);
  p_packet += sizeof(Network::PacketType);
  bytes_read -= sizeof(Network::PacketType);

  if (packet_type == Network::PacketType::PT_Command)
    {
    size_t offset = 0;
    int command_id = FromChar<int>(p_packet);
    offset += sizeof(int);
    float x_pos = FromChar<float>(p_packet + offset);
    offset += sizeof(float);
    float y_pos = FromChar<float>(p_packet + offset);

    MetaFactory& factory = ServerMain::GetInstance().GetMetaFactory();
    int type_id = -1;
    if (command_id == Commands::CreateManufacture)
      type_id = factory.GetObjectID("BasePlugin.ManufactureObject");
    else if (command_id == Commands::CreateStore)
      type_id = factory.GetObjectID("BasePlugin.ProductionStoreObject");

    if (type_id != -1)
      GetGameContext().AddObject(type_id, Vector2D(x_pos, y_pos), GetMask());
    }
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