#pragma once
#define NOMINMAX

#include <boost\utility.hpp>
#include <cassert>
#include <limits>
//////////////////////////////////////////////////////////////////////////

class MathUtilities : boost::noncopyable
{
private:
  MathUtilities();
  ~MathUtilities();

public:
  template <typename T>
  static bool IsEqual(const T& i_first, const T& i_second);

  //clamps the first argument between the second two
  template <typename T, typename U, typename V>
  static void Clamp(T& arg, const U& minVal, const V& maxVal);
};


template <typename T>
bool MathUtilities::IsEqual(const T& i_first, const T& i_second)
{
  return i_first - i_second < std::numeric_limits<T>::min();
}

template <typename T, typename U, typename V>
void MathUtilities::Clamp(T& arg, const U& minVal, const V& maxVal)
{
  assert ( (minVal < maxVal) && "<Clamp>MaxVal < MinVal!");

  if (arg < (T)minVal)
  {
    arg = (T)minVal;
  }

  if (arg > (T)maxVal)
  {
    arg = (T)maxVal;
  }
}