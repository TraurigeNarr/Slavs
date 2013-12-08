#pragma once

#include <boost/any.hpp>


template <typename Identifier>
class Property
{
private:
  Identifier m_type;
  boost::any m_value;
public:
  Property(Identifier i_id, boost::any&& i_value);
  template <typename ValueType>
  Property(Identifier i_id, const ValueType& i_value);
  ~Property();

  Identifier  GetType() const;

  boost::any  GetProperty() const;
  
  void        SetProperty(boost::any&& i_value);

  template <typename ValueType>
  void        SetProperty(const ValueType& i_value);

  bool SameType(Identifier i_type) const;
  bool operator == (const Property<Identifier>& i_other) const;
};

template <typename Identifier>
Property<Identifier>::Property(Identifier i_id, boost::any&& i_value)
  : m_type(i_id)
  , m_value(i_value)
{

}

template <typename Identifier>
template <typename ValueType>
Property<Identifier>::Property(Identifier i_id, const ValueType& i_value)
  : m_type(i_id)
  , m_value(i_value)
{

}

template <typename Identifier>
Property<Identifier>::~Property()
{

}

template <typename Identifier>
Identifier Property<Identifier>::GetType() const
{
  return m_type;
}

template <typename Identifier>
boost::any Property<Identifier>::GetProperty() const
{
  return m_value;
}


template <typename Identifier>
void Property<Identifier>::SetProperty(boost::any&& i_value)
{
  m_value = i_value;
}


template <typename Identifier>
template <typename ValueType>
void Property<Identifier>::SetProperty(const ValueType& i_value)
{
  m_value = i_value;
}

template <typename Identifier>
bool Property<Identifier>::SameType(Identifier i_type) const
{
  return m_type == i_type;
}

template <typename Identifier>
bool Property<Identifier>::operator == (const Property<Identifier>& i_other) const
{
  return m_type == i_other.m_id && 
        (boost::unsafe_any_cast<void*>(&m_value)) == (boost::unsafe_any_cast<void*>(&i_other.m_value));
}