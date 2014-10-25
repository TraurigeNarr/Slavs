#pragma once

class BitMask
{
private:
  int m_mask;


public:
  BitMask();
  ~BitMask();

  void Reset(char i_bit_number);
  void ResetAll();
  void Flip();
  void Set(char i_bit_number);
  bool Has(char i_bit_number) const;
  bool Empty() const;
};