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
int ToChar(T val, char* buf, int size)
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
T FromChar(char* val)
{
	return NULL != val ? *(T*)val : NULL;
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