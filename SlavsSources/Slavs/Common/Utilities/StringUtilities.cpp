#include "StringUtilities.h"

#include <algorithm>
#include <vector>
/////////////////////////////////////////////////////////

StringUtilities::StringUtilities()
{
}

StringUtilities::~StringUtilities()
{
}

bool StringUtilities::AllDigits(const std::string& i_text)
{
	for (size_t i = 0; i < i_text.length(); i++)
	{
		if (!isdigit(i_text[i]))
			return false;
	}

	return true;
}

std::string StringUtilities::ToLower(const std::string& i_value)
{
	std::string valueLower;

	std::for_each(i_value.begin(), i_value.end(), [&valueLower](char c)
	{
		valueLower += tolower(c);
	});
	return valueLower;
}

std::string StringUtilities::GetAsString(std::iostream& i_stream)
{
	int length;
	char* p_buffer;
	
	// get length of file:
	i_stream.seekg (0, std::iostream::end);
	length = static_cast<int>(i_stream.tellg());
	i_stream.seekg (0, std::iostream::beg);

	// allocate memory:
	p_buffer = new char [length];
	//write file
	int pos = 0;
	while(i_stream.good())
	{
		p_buffer[pos] = i_stream.get();
		++pos;
	}

	std::string out(p_buffer, pos);
	delete[] p_buffer;
	return out;
}

bool StringUtilities::ParseBool(const std::string& i_value)
{
  	std::string value_lower = StringUtilities::ToLower(i_value);
	
	if (value_lower .empty() || value_lower  == "false" || value_lower  == "no" || value_lower  == "0")
		return false;
	else
		return true;
}

std::vector<std::string> StringUtilities::Tokenize(const std::string& i_input_string, const std::string& i_separators)
  {
  std::vector<std::string> tokens;

  std::string input(i_input_string);
  std::string::size_type idx = 0;

  idx = input.find_first_not_of(i_separators);
  // nothing found here
  if (idx == std::string::npos)
    return tokens;

  while (idx != std::string::npos)
    {
    std::string::size_type token_end_idx = input.find_first_of(i_separators, idx + 1);
    tokens.push_back(input.substr(idx, token_end_idx - idx));
    if (token_end_idx == std::string::npos)
      break;
    idx = input.find_first_not_of(i_separators, token_end_idx + 1);
    }

  return tokens;
  }