#include "stdafx.h"

#include "LoadingStages.h"
#include "LoadingState.h"
#include "IMessageProvider.h"

#include "ClientGameContext.h"

#include "TimeUtilities.h"

#include <Network/PacketProvider.h>
#include <Network/PacketType.h>
#include <Network/Packet.h>
#include <Network/include/Connection.h>

#include <Common/Utilities/TemplateFunctions.h>
#include <Common/Patterns/StateMachine.h>

#include <Common/Game/GameObjectState.h>

//////////////////////////////////////////////////////////////////////////

using namespace ClientStates;
using namespace LoadingStages;
using namespace net;
using namespace Network;

//////////////////////////////////////////////////////////////////////////
//ConnectionState

ConnectionState::ConnectionState (Network::PacketProvicer& i_packet_provider)
  : m_packet_provider(i_packet_provider)
  {  }

void ConnectionState::Enter(LoadingState* ip_owner)
  {
  Connection& connection = m_packet_provider.GetConnection();

  connection.Connect(ip_owner->GetConnectionAddress());
  std::unique_ptr<char[]> p_buffer(new char[sizeof(Network::PacketType)]);
  ToChar(Network::PacketType::PT_Achived, &p_buffer[0], sizeof(Network::PacketType));
  m_packet_provider.GetConnection().SendPacket(&p_buffer[0], sizeof(Network::PacketType));
  ip_owner->GetMessageProvider()->Invalidate();
  }

void ConnectionState::Execute(LoadingState* ip_owner, long i_parameter)
  {
  m_packet_provider.GetConnection().Update(TimeUtilities::ConvertTime(i_parameter));
  m_packet_provider.GetNextPacket();
  //check for connection
  if(m_packet_provider.GetConnection().ConnectFailed())
    {
    ip_owner->GetStateMachine().ChangeState(std::make_shared<ResultsState>(LoadingResult::LR_FAILED));
    return;
    }
  else if (m_packet_provider.GetConnection().IsConnected())
    {
    ip_owner->GetStateMachine().ChangeState(std::make_shared<GettingData>(m_packet_provider));
    return;
    }
  }

void ConnectionState::Exit(LoadingState* ip_owner)
  {  }

//////////////////////////////////////////////////////////////////////////
// FailedState

ResultsState::ResultsState (LoadingResult i_result)
  : m_result(i_result)
  {  }

void ResultsState::Enter(LoadingState* ip_owner)
  {
  ip_owner->GetMessageProvider()->Invalidate();
  }

void ResultsState::Execute(LoadingState* ip_owner, long i_parameter)
  {  }

void ResultsState::Exit(LoadingState* ip_owner)
  {  }

LoadingResult ResultsState::GetResult() const
  {
  return m_result;
  }

//////////////////////////////////////////////////////////////////////////
// GettingData

GettingData::GettingData (Network::PacketProvicer& i_packet_provider)
  : m_packet_provider(i_packet_provider)
  , m_current_data_type(CurrentData::CD_NOTHING)
  , mp_owner(nullptr)
  {  }

void GettingData::Enter(LoadingState* ip_owner)
  {
  m_current_data_type = CurrentData::CD_DEFINITIONS;
  m_content_to_load_number = 0;
  m_current_content_number = 0;
  mp_owner = ip_owner;
  ip_owner->GetMessageProvider()->Invalidate();
  }

void GettingData::Execute(LoadingState* ip_owner, long i_parameter)
  { 
  Packet packet = m_packet_provider.GetNextPacket();

  if (packet())
    {
    switch (m_current_data_type)
      {
      case GettingData::CurrentData::CD_DEFINITIONS:
        _HandleDefinition(packet);
        ip_owner->GetMessageProvider()->Invalidate();
        break;
      case GettingData::CurrentData::CD_OBJECTS:
        _HandleObject(packet);
        ip_owner->GetMessageProvider()->Invalidate();
        break;
      }
    }
  if (m_current_data_type == GettingData::CurrentData::CD_ALL_LOADED)
    ip_owner->GetStateMachine().ChangeState(std::make_shared<ResultsState>(LoadingResult::LR_SUCCEEDED));
  }

void GettingData::Exit(LoadingState* ip_owner)
  { 
  }

void GettingData::_HandleDefinition(const Packet& i_packet)
  {
  switch (i_packet.m_packet)
    {
    case Network::PacketType::PT_ContentNumber:
      m_content_to_load_number = ConvertTo<size_t>(i_packet.mp_data);
      m_current_content_number = 0;
      break;
    case Network::PacketType::PT_Definitions:
      {
      std::string obj_name  = "";
      int         obj_id    = -1;
      const char* const p_data = static_cast<const char* const>(i_packet.mp_data);
      for (size_t i = 0; i < i_packet.m_size; ++i)
        {
        if (p_data[i] == ';')
          {
          obj_id = FromChar<int>(p_data+i+1);
          break;
          }
        else
          obj_name += p_data[i];
        }

      if (obj_id != -1)
        mp_owner->GetContext().AddDefinition(std::make_pair(obj_name, obj_id));

      ++m_current_content_number;
      if (m_current_content_number == m_content_to_load_number)
        {
        mp_owner->GetContext().Initialize();
        m_current_data_type = GettingData::CurrentData::CD_OBJECTS;
        _SendReadyPacket();
        }
      }
      break;
    }
  }

void GettingData::_HandleObject(const Packet& i_packet)
  {
  switch (i_packet.m_packet)
    {
    case Network::PacketType::PT_ContentNumber:
      m_content_to_load_number = ConvertTo<size_t>(i_packet.mp_data);
      m_current_content_number = 0;
      break;
    case Network::PacketType::PT_GOState:
      {
      GameObjectState state;
      const char* const p_buffer = static_cast<const char* const>(i_packet.mp_data);
      state.Deserialize(const_cast<char*>(p_buffer));
      mp_owner->GetContext().ApplyState(state);

      ++m_current_content_number;
      if (m_current_content_number == m_content_to_load_number)
        {
        m_current_data_type = GettingData::CurrentData::CD_ALL_LOADED;
        _SendReadyPacket();
        }
      }
      break;
    }
  }

void GettingData::_SendReadyPacket() const
  {
  Network::PacketType packet_type = Network::PacketType::PT_Achived;
  std::unique_ptr<unsigned char[]> p_buf(new unsigned char[sizeof(Network::PacketType)]);
  ToChar(packet_type, &p_buf[0], sizeof(Network::PacketType));
  m_packet_provider.GetConnection().SendPacket(p_buf.get(), sizeof(Network::PacketType));
  }

size_t GettingData::GetLoadedContentNumber() const
  {
  return m_current_content_number;
  }

size_t GettingData::GetTotalContentNumber() const
  {
  return m_content_to_load_number;
  }

GettingData::CurrentData GettingData::GetStage() const
  {
  return m_current_data_type;
  }

//////////////////////////////////////////////////////////////////////////
