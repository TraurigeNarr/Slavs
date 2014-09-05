#include "SendController.h"

#include <algorithm>

using namespace net;

SendController::SendController()
{

}

SendController::~SendController()
{
  //release controller ownership
  std::for_each(m_senders.begin(), m_senders.end(), [](TSender ip_sender)
  {
    ip_sender = nullptr;
  });

  m_senders.clear();
}

void SendController::Update(long i_elapsed_time)
{
  std::for_each(m_senders.begin(), m_senders.end(), [i_elapsed_time](TSender ip_sender)
  {
    Packet p(PacketType::PT_None, nullptr);
    ip_sender->Update(i_elapsed_time);
    if (ip_sender->HasChanges())
      ip_sender->Serialize(p);
  });
}

void SendController::RegisterSender(TSender ip_sender)
{
  std::vector<TSender>::iterator it = std::find(m_senders.begin(), m_senders.end(), ip_sender);

  if(it == m_senders.end())
    m_senders.push_back(ip_sender);
}

void SendController::UnregisterSender(TSender ip_sender)
{
  std::remove_if(m_senders.begin(), m_senders.end(), [ip_sender](TSender sender) -> bool
  {
    return sender == ip_sender;
  });
}