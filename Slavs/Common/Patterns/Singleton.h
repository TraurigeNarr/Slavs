#pragma once

#include <cassert>
#include <memory>
#include <utility>

///////////////////////////////////////////////////////////

/*
Purpose:  Realization of Singleton pattern.
This class incapsulates object of class T that need to be singleton.
Usage: define in .cpp file such

template<> std::shared_ptr<T> Singleton<Application>::mp_singleton = nullptr;

where T is your type. Further use GetInstance method. If you need to destroy object call Release method.
*/

template <class T>
class Singleton
{
private:
  static std::shared_ptr<T> mp_singleton;
private:
  Singleton(){}
  Singleton(const Singleton& i_other);

  Singleton& operator = (const Singleton& i_other);
public:
  //takes control over i_instance
  Singleton(T* i_instance)
  {
    assert(nullptr == mp_singleton.get());
    mp_singleton = std::shared_ptr<T>(i_instance);
  }

  ~Singleton()
  {
    assert(nullptr != mp_singleton.get());
    mp_singleton.reset();
  }

  static T& GetInstance()
  {
    assert(nullptr != mp_singleton.get());
    return *mp_singleton.get();
  }

  static std::shared_ptr<T> GetInstancePtr()
  {
    return mp_singleton;
  }

  static void Release()
  {
    assert(nullptr != mp_singleton.get());
    mp_singleton.reset();
  }

  static void ReleaseIfValid()
  {
    if(IsValid())
      Release();
  }

  static bool IsValid()
  {
    return mp_singleton.get() != nullptr;
  }
};