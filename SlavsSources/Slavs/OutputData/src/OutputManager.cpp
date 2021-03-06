#include "OutputManager.h"

#include "ISerializable.h"

#include <algorithm>
#include <fstream>
#include <Windows.h>
#include <iostream>

//////////////////////////////////////////////////////////////////////////

namespace
{
  void CreateDirectoryIfNotExist(const std::wstring& i_directory_name)
  {
    CreateDirectory(i_directory_name.c_str(), LPSECURITY_ATTRIBUTES());

    if(GetLastError() == ERROR_PATH_NOT_FOUND)
      throw std::exception("One or more intermediate directories do not exist\n");
  }

  std::wstring GetNextFileName(const std::wstring& i_file_name)
  {
    return std::wstring();
  }
}//namespace

//////////////////////////////////////////////////////////////////////////

OutputManager::OutputManager(const std::wstring& i_directory, bool i_all_data_in_one_file)
  : m_directory_name(i_directory), m_all_in_one_file(i_all_data_in_one_file)
{

}

//////////////////////////////////////////////////////////////////////////

OutputManager::~OutputManager()
{
  m_data_items.clear();
}

//////////////////////////////////////////////////////////////////////////

void OutputManager::AddItem(std::shared_ptr<ISerializable> i_item)
{
  m_data_items.push_back(i_item);
}

//////////////////////////////////////////////////////////////////////////

void OutputManager::WriteData()
{
  try
  {
    CreateDirectoryIfNotExist(m_directory_name);
  }
  catch(std::exception& i_e)
  {
    std::cerr << i_e.what();
    m_data_items.clear();
    return;
  }

  std::wstring file_name = m_directory_name + L"\\data.txt";

  
  std::wofstream ofs(file_name.c_str());

  if(ofs.bad() || ofs.fail() || ERROR_ACCESS_DENIED == GetLastError())
  {
    ofs.close();
    throw std::exception("File was damaged or not created via access denied\n");
  }
  ofs << "This file was generated by SlavsServer. You can pass this data to Excel, or another program and use it how you wish." 
    << std::endl
    << "//////////////////////////////////////////////////////////////////////////"
    << std::endl;

  for(size_t i = 0; i < m_data_items.size(); ++i)
  {
    ofs << *m_data_items[i].get();
    ofs << std::endl;
  }

  /*std::for_each(m_data_items.begin(), m_data_items.end(), [&ofs](std::shared_ptr<ISerializable> i_item)
  {
    ofs << (*i_item.get());
  });*/
  m_data_items.clear();

  ofs.close();
  std::cout << "Closed successfully" << std::endl;
}