#pragma once

#include <algorithm>
#include <stddef.h>
#include <vector>

template<class T> 
void swap(T& a, T& b) 
{ 
  T tmp(std::move(a));
  a = std::move(b); 
  b = std::move(tmp);
} 

template < typename T>
char* ToChar(T& val)
{
	char *c = new char[sizeof(T)];
	char *p = (char*)&val;
	for(int i = 0; i < sizeof(T); ++i)
	{
		c[i] = *(p + i);
	}
	return c;
}

template<typename T>
T Round(float i_value)
{
  return static_cast<T>(  static_cast<unsigned int>(i_value + 0.49999997f) );
}

template < typename T>
size_t ToChar(T val, char* op_buf, size_t i_size)
{
	char *p = (char*)&val;
	size_t i = 0;
	for(i = 0; i < i_size; ++i)
	{
		op_buf[i] = *(p + i);
	}
	return i;
}

template < typename T>
size_t ToChar(const T* val, void* op_buf, size_t i_size)
  {
  const char* p_source = reinterpret_cast<const char*>(val);
  char* p_dest  = static_cast<char*>(op_buf);
  size_t i = 0;
  for(i = 0; i < i_size; ++i)
    {
    *(p_dest + i) = *(p_source + i);
    }
  return i;
  }

template < typename T>
int ToChar(T& val, unsigned char* buf, int size)
  {
  char *p = (char*)&val;
  int i = 0;
  for(i = 0; i < size; ++i)
    {
    buf[i] = *(p + i);
    }
  return i;
  }

template < typename T>
T FromChar(const char* const val)
{
	return NULL != val ? *(T*)val : NULL;
}

template < typename T>
size_t FromChar(const char* const ip_source, T& o_destination)
  {
  o_destination = *reinterpret_cast<const T*>(ip_source);
  return sizeof(T);
  }

template < typename T>
size_t FromChar(const unsigned char* const ip_source, T& o_destination)
  {
  memcpy(&o_destination, ip_source, sizeof(T));
  return sizeof(T);
  }

template < typename ConversionTarget>
ConversionTarget ConvertTo(const void* i_buffer)
  {
  return nullptr != i_buffer ? *reinterpret_cast<const ConversionTarget*>(i_buffer) : ConversionTarget();
  }

template < typename ConversionTarget>
ConversionTarget ConvertFrom(const void* i_buffer)
  {
  return nullptr != i_buffer ? *reinterpret_cast<const ConversionTarget*>(i_buffer) : ConversionTarget();
  }

template <class T>
void ClearVector(std::vector<T*>& vec) 
{
	while(!vec.empty()) 
	{
  T* obj = vec.back();
  vec.pop_back();
  delete obj;
	}
	vec.clear();
}

template <class map>
inline void ClearSTLMap(map& m)
{
	for (map::iterator it = m.begin(); it!=m.end(); ++it)
	{
		delete it->second;
		it->second = NULL;
	}
}

template <typename Container, typename Key, typename Value>
Key FindKeyByValue(const Container& i_container, Value& i_value)
  {
  Container::const_iterator it = std::find_if(i_container.begin(), i_container.end(), [&i_value](const std::pair<Key, Value>& i_pair)
    {
    return i_pair.second == i_value;
    });
  assert (it != i_container.end());
  return (*it).first;
  }