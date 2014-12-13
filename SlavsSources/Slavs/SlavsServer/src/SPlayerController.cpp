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
	, m_object_selector(*this, m_selected_objects)
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

void SPlayerController::HoldPacket(unsigned char* packet, int bytes_read)
  {
	unsigned char* p_packet = packet;
  Network::PacketType packet_type = (Network::PacketType)ConvertTo<int>(p_packet);
  p_packet += sizeof(Network::PacketType);
  bytes_read -= sizeof(Network::PacketType);

  if (packet_type == Network::PacketType::PT_Selection)
		m_object_selector.SelectObjects(p_packet, bytes_read);
  }