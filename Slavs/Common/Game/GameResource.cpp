#include "GameResource.h"

#include <algorithm>


GameResource::GameResource(GResourceType i_type, GResourceSubType i_subtype)
  : m_type(i_type)
  , m_subtype(i_subtype)
{

}

GameResource::~GameResource()
{
  m_properties.clear();
}

void GameResource::SetProperty(GResourceProperty i_type, boost::any&& i_value)
{
  TProperty p = TProperty(i_type, i_value);
  TProperties::iterator it = std::find_if(m_properties.begin(), m_properties.end(), [i_type](const TProperty& prop) -> bool
  {
    return prop.SameType(i_type);
  });
  if (it == m_properties.end())
    m_properties.push_back(TProperty(i_type, i_value));
  else
    it->SetProperty(i_value);
}

boost::any GameResource::GetProperty(GResourceProperty i_type)
{
  TProperties::iterator it = std::find_if(m_properties.begin(), m_properties.end(), [i_type](const TProperty& prop) -> bool
  {
    return prop.SameType(i_type);
  });
  if (it != m_properties.end())
    return it->GetProperty();
  return boost::any();
}

GResourceType GameResource::GetType() const
{
  return m_type;
}

GResourceSubType GameResource::GetSubtype() const
{
  return m_subtype;
}