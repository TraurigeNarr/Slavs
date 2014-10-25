#pragma once

#include <boost\utility.hpp>

#include <iostream>
#include <sstream>
#include <string>
/////////////////////////////////////////////////////

class StringUtilities : boost::noncopyable
{
private:
  StringUtilities();
  ~StringUtilities();

public:
  static bool					AllDigits(const std::string& i_text);
	static std::string  ToLower(const std::string& i_value);
  //read all data in stream in string
	static std::string	GetAsString(std::iostream& i_stream);
  static bool         ParseBool(const std::string& i_value);
  template <typename T>
  static std::string t_to_string(T&& i_value);
};

template <typename T>
std::string StringUtilities::t_to_string(T&& i_value)
{
    std::stringstream s_stream;
    std::string s;
    s_stream << i_value;
    s = s_stream.str();

    return s;
}