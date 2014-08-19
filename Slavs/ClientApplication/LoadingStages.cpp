#include "stdafx.h"

#include "LoadingStages.h"
#include "LoadingState.h"

#include "TimeUtilities.h"

#include "PacketProvider.h"
#include "Packet.h"

#include <Network/include/Connection.h>

#include <Common/Utilities/TemplateFunctions.h>
#include <Common/Patterns/StateMachine.h>

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
  std::unique_ptr<char[]> p_buffer(new char[sizeof(PacketType)]);
  ToChar(PacketType::PT_Achived, &p_buffer[0], sizeof(PacketType));
  m_packet_provider.GetConnection().SendPacket(&p_buffer[0], sizeof(PacketType));
  }

void ConnectionState::Execute(LoadingState* ip_owner, long i_parameter)
  {
  m_packet_provider.GetConnection().Update(TimeUtilities::ConvertTime(i_parameter));

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
  {  }

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
  {  }

void GettingData::Enter(LoadingState* ip_owner)
  {
  m_content_number = 0;
  m_current_data_type = CurrentData::CD_DEFINITIONS;
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
        break;
      case GettingData::CurrentData::CD_OBJECTS:
        _HandleObject(packet);
        break;
      }
    }
  }

void GettingData::Exit(LoadingState* ip_owner)
  { 
  }

void GettingData::_HandleDefinition(const Packet& i_packet)
  {
  switch (i_packet.m_packet)
    {
    case PacketType::PT_ContentNumber:
      m_content_number = ConvertTo<size_t>(i_packet.mp_data);
      m_current_content_number = 0;
      break;
    case PacketType::PT_Definitions:
      ++m_current_content_number;
      if (m_current_content_number == m_content_number)
        {
        m_current_data_type = GettingData::CurrentData::CD_OBJECTS;
        _SendReadyPacket();
        }
      break;
    }
  }

void GettingData::_HandleObject(const Packet& i_packet)
  {
  switch (i_packet.m_packet)
    {
    case PacketType::PT_ContentNumber:
      m_content_number = ConvertTo<size_t>(i_packet.mp_data);
      m_current_content_number = 0;
      break;
    case PacketType::PT_GOState:
      ++m_current_content_number;
      if (m_current_content_number == m_content_number)
        _SendReadyPacket();
      break;
    }
  }

void GettingData::_SendReadyPacket() const
  {
  PacketType packet_type = PacketType::PT_Achived;
  std::unique_ptr<unsigned char[]> p_buf(new unsigned char[sizeof(PacketType)]);
  ToChar(packet_type, &p_buf[0], sizeof(PacketType));
  m_packet_provider.GetConnection().SendPacket(p_buf.get(), sizeof(PacketType));
  }
