#pragma once

template <typename CoordinateType, size_t Dimension>
class Vector
  {
  private:
    std::array<CoordinateType,Dimension> m_coordinates;

  public:
    Vector()
      {
      for(int i = 0; i < Dimension; ++i)
        m_coordinates[i] = CoordinateType();
      }

    template <typename NewType>
    explicit Vector(const Vector<NewType,Dimension>& i_vector)
      {
      for(int i = 0; i < Dimension; ++i)
        m_coordinates[i] = static_cast<CoordinateType>(i_vector[i]);
      }

    CoordinateType LengthSq()
      {
      CoordinateType length = CoordinateType();
      for(int i = 0; i < Dimension; ++i)
        length += m_coordinates[i] * m_coordinates[i];
      return length;
      }

    CoordinateType Length()
      {
      return sqrt (LengthSq());
      }

    void Normalize()
      {
      CoordinateType length = Length();
      this->operator/= (length);
      }

    Vector& operator += (const Vector<CoordinateType,Dimension>& i_vec)
      {
      for(int i = 0; i < Dimension; ++i)
        m_coordinates[i] += i_vec.m_coordinates[i];
      return *this;
      }

    Vector operator -= (const Vector<CoordinateType,Dimension>& i_vec)
      {
      for(int i = 0; i < Dimension; ++i)
        m_coordinates[i] -= i_vec.m_coordinates[i];
      return *this;
      }

    Vector operator + (const Vector<CoordinateType,Dimension>& i_vec)
      {
      Vector<T, size> tmp(i_vec);
      return tmp+=*this;
      }

    Vector operator - (const Vector<CoordinateType,Dimension>& i_vec)
      {
      Vector<CoordinateType, size> tmp(i_vec);
      return tmp -= *this;
      }

    const Vector<CoordinateType, Dimension>& operator*=(const CoordinateType& i_multiplier)
      {
      for (size_t i = 0; i < Dimension; ++i)
        m_coordinates[i] *= i_multiplier;
      return *this;
      }

    const Vector<CoordinateType, Dimension>& operator/=(const CoordinateType& i_devider)
      {
      for (size_t i = 0; i < Dimension; ++i)
        m_coordinates[i] /= i_devider;
      return *this;
      }

    CoordinateType operator [] (int i_index) const
      {
      return m_coordinates[i_index];
      }

    CoordinateType& operator [] (int i_index)
      {
      return m_coordinates[i_index];
      }

    bool operator == (const Vector<CoordinateType, Dimension>& i_vec)
      {
      for(int i = 0; i < Dimension; ++i)
        if(abs(m_coordinates[i] - i_vec.m_coordinates[i]) > std::numeric_limits<T>::min())
          return false;
      return true;
      }

    bool IsZero() const
      {
      for(int i = 0; i < Dimension; ++i)
        if(abs(m_coordinates[i]) > std::numeric_limits<T>::min())
          return false;
      return true;
      }
  };