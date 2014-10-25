#include "BitMask.h"

BitMask::BitMask()
  : m_mask(0)
{

}

BitMask::~BitMask()
{

}

void BitMask::ResetAll()
{
  m_mask = 0;
}

void BitMask::Reset(char i_bit_number)
{
  m_mask = m_mask & ~(1<<i_bit_number);
}

bool BitMask::Empty() const
{
  return m_mask == 0;
}

void BitMask::Flip()
{
  m_mask = ~m_mask;
}

void BitMask::Set(char i_bit_number)
{
  m_mask |= i_bit_number;
}

bool BitMask::Has(char i_bit_number) const
{
  return 0 != (m_mask & i_bit_number);
}