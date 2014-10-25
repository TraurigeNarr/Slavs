#include "ISerializable.h"

//////////////////////////////////////////////////////////////////////////

std::wostream& operator << (std::wostream& i_s, const ISerializable& i_data)
{
  return i_data.WriteInStream(i_s);
}

//////////////////////////////////////////////////////////////////////////

ISerializable::ISerializable()
{

}

ISerializable::~ISerializable()
{

}