#pragma once

#include <string>
#include <vector>
#include <memory>
#include <ostream>

class ISerializable;


/*
This class contains data, that should be written in files.
*/
class OutputManager
{
private:
  std::vector<std::shared_ptr<ISerializable>> m_data_items;

  std::wstring m_directory_name;
  bool m_all_in_one_file;
public:
  /*
  @param i_directory - in what directory should output
  @param i_all_data_in_one_file - if true, then in directory creates/overwrites file data.txt
  if @param is false, then several files created, one for each ISerializable item
  */
  OutputManager(const std::wstring& i_directory, bool i_all_data_in_one_file);
  ~OutputManager();

  void AddItem(std::shared_ptr<ISerializable> i_item);

  //out all data in defined directory and files and flush buffers, 
  //throws exception if directory or file can not be created/opened
  void WriteData();
};