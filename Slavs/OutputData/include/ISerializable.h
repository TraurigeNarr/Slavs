#pragma once

#include <ostream>

class ISerializable
{
private:
  friend std::wostream& operator << (std::wostream&, const ISerializable&);
  virtual std::wostream& WriteInStream(std::wostream&) const = 0;
public:
  ISerializable();
  virtual ~ISerializable();
};