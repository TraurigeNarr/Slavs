#pragma once

#include "GameResourceTypes.h"

#include "../misc/Property.h"

#include <vector>


class GameResource
{
  typedef Property<GResourceProperty> TProperty;
  typedef std::vector<TProperty> TProperties; 
private:
  GResourceType     m_type;
  GResourceSubType  m_subtype;
  TProperties       m_properties;
public:
  GameResource(GResourceType i_type, GResourceSubType i_subtype);
  ~GameResource();

  void              SetProperty(GResourceProperty i_type, boost::any&& i_value);
  template <typename ValueType>
  void              SetProperty(GResourceProperty i_type, const ValueType& i_value);

  boost::any        GetProperty(GResourceProperty i_type);

  GResourceType     GetType() const;
  GResourceSubType  GetSubtype() const;
};

template <typename ValueType>
void GameResource::SetProperty(GResourceProperty i_type, const ValueType& i_value)
{
  TProperties::iterator it = std::find(m_properties.begin(), m_properties.end(), i_value);
  if (it == m_properties.end())
    m_properties.push_back(TProperty(i_type, i_value));
  else
    it->SetProperty(i_value);
}