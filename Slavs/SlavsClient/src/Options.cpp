#include "Options.h"

#include <Net.h>

Options::Options()
{
  m_server_address = new net::Address(127,0,0,1,30000);
}

Options::~Options()
{
  delete m_server_address;
}

void Options::SetServerAddress(const net::Address& i_address)
{
  delete m_server_address;
  m_server_address = new net::Address(i_address);
}

void Options::SetOption(const std::string& i_option_name, const std::string& i_option_value)
{
  m_Options[i_option_name] = i_option_value;
}

std::string Options::GetOption(const std::string& i_name) const
{
  if(m_Options.end() == m_Options.find(i_name))
    return "";
  else
    return m_Options.find(i_name)->second;
}

net::Address Options::GetServerAddress() const
{
  return *m_server_address;
}