#include "DllManager.h"

#include <Utilities/TemplateFunctions.h>

#include <Windows.h>

//////////////////////////////////////////////////////////////////////////
// DllManager::DynLibraryHandle

typedef void (*DllFunction)();

class DllManager::DynLibraryHandle
 {
 private:
   HINSTANCE m_instance;

 public:
   DynLibraryHandle();
   DynLibraryHandle(const std::wstring& i_filename);
   ~DynLibraryHandle();

   void Load(const std::wstring& i_filename);
   void Unload();
 };

DllManager::DynLibraryHandle::DynLibraryHandle()
  : m_instance(nullptr)
  {  }

DllManager::DynLibraryHandle::DynLibraryHandle(const std::wstring& i_filename)
  : m_instance(nullptr)
  {
  Load(i_filename);
  }

DllManager::DynLibraryHandle::~DynLibraryHandle()
  {
  Unload();
  }

void DllManager::DynLibraryHandle::Load(const std::wstring& i_filename)
  {
  std::wstring lib_name = i_filename;
  if (lib_name.substr(lib_name.length() - 4, 4) != L".dll")
    lib_name += L".dll";
  
  m_instance = LoadLibraryEx(lib_name.c_str(), NULL, LOAD_LIBRARY_SEARCH_APPLICATION_DIR);

  if (m_instance == NULL)
    return;
  DllFunction startup = (DllFunction)GetProcAddress(m_instance, "dllStartup");
  if (startup == NULL)
    return;
  startup();
  }

void DllManager::DynLibraryHandle::Unload()
  {
  if (m_instance != nullptr)
    {
    DllFunction shudDown = (DllFunction)GetProcAddress(m_instance, "dllShutdown");
    if (shudDown == NULL)
      return;
    shudDown();
    FreeLibrary(m_instance);
    m_instance = nullptr;
    }
  }

//////////////////////////////////////////////////////////////////////////
// DllManager

DllManager::DllManager(const std::wstring& i_libs_directory)
  : m_libs_directory(i_libs_directory)
  {  }

DllManager::~DllManager()
  {
  ReleaseLibraries();
  }

void DllManager::LoadLib(const std::wstring& i_filename)
  {
  TLibraries::iterator it_lib = m_libraries.find(i_filename);

  if (it_lib == m_libraries.end())
    {
    m_libraries[i_filename] = new DynLibraryHandle(m_libs_directory + L"\\" + i_filename);
    }
  }

void DllManager::ReleaseLibraries()
  {
  ClearSTLMap(m_libraries);
  m_libraries.clear();
  }