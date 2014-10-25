#pragma once
#include <OgreCommon.h>

namespace net
{
  class Address;
}

#include <string>

//////////////////////////////////////////////////////////////////////////
class Options
{
public:
  Options();
  ~Options();

  void          SetOption(const std::string& i_option_name, const std::string& i_option_value);
  void          SetServerAddress(const net::Address& i_address);

  std::string   GetOption(const std::string& i_name) const;
  net::Address  GetServerAddress() const;
private:
  Ogre::NameValuePairList m_Options;

  net::Address* m_server_address;
};