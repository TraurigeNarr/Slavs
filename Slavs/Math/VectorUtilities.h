#pragma once

#include "Vector.h"

template <typename CoordinateType, size_t Dimension>
inline void Truncate (Vector<CoordinateType, Dimension>& io_vector, const CoordinateType& i_max)
  {
  if (io_vector.LengthSq() > i_max*i_max)
    {
    io_vector.Normalize();
    io_vector *= i_max;
    }
  }

template <typename CoordinateType, size_t Dimension>
inline Vector<CoordinateType, Dimension> ReverseVector(const Vector<CoordinateType, Dimension>& i_vector)
  {
  Vector<CoordinateType, Dimension> result_vector;
  for (size_t i = 0; i < Dimension; ++i)
    result_vector[i] = -i_vector[i];

  return result_vector;
  }

template <typename CoordinateType, size_t Dimension>
inline CoordinateType Vec2DDistance(const Vector<CoordinateType, Dimension>& i_vector_from, const Vector<CoordinateType, Dimension>& i_vector_to)
  {
  CoordinateType distance_sq = CoordinateType();
  for (size_t i = 0; i < Dimension; ++i)
    {
    CoordinateType i_coordinate_separation = i_vector_to[i] - i_vector_from[i];
    distance_sq += i_coordinate_separation * i_coordinate_separation;
    }

  return sqrt(distance_sq);
  }

//////////////////////////////////////////////////////////////////////////
// arithmetic operators

template <typename CoordinateType, size_t Dimension>
inline Vector<CoordinateType, Dimension> operator*(const Vector<CoordinateType, Dimension> &lhs, float rhs)
  {
  Vector<CoordinateType, Dimension> result(lhs);
  result *= rhs;
  return result;
  }

template <typename CoordinateType, size_t Dimension>
inline Vector<CoordinateType, Dimension> operator*(float lhs, const Vector<CoordinateType, Dimension> &rhs)
  {
  Vector<CoordinateType, Dimension> result(rhs);
  result *= lhs;
  return result;
  }

template <typename CoordinateType, size_t Dimension>
inline Vector<CoordinateType, Dimension> operator-(const Vector<CoordinateType, Dimension> &lhs, const Vector<CoordinateType, Dimension> &rhs)
  {
  Vector<CoordinateType, Dimension> result(lhs);
  result.x -= rhs.x;
  result.y -= rhs.y;

  return result;
  }

template <typename CoordinateType, size_t Dimension>
inline Vector<CoordinateType, Dimension> operator+(const Vector<CoordinateType, Dimension> &lhs, const Vector<CoordinateType, Dimension> &rhs)
  {
  Vector<CoordinateType, Dimension> result(lhs);
  result += rhs;
  return result;
  }

template <typename CoordinateType, size_t Dimension>
inline Vector<CoordinateType, Dimension> operator/(const Vector<CoordinateType, Dimension> &lhs, float val)
  {
  Vector<CoordinateType, Dimension> result(lhs);
  result /= val;

  return result;
  }

//////////////////////////////////////////////////////////////////////////