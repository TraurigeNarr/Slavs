#pragma once

#include "SlavsServerAPI.h"

#include <map>
#include <memory>
#include <string>

class SLAVS_SERVER_EXPORT DllManager
  {
  private:
    class DynLibraryHandle;
    
    typedef std::map<std::wstring, DynLibraryHandle*>  TLibraries;

  private:
    std::wstring m_libs_directory;
    TLibraries m_libraries;


  public:
    DllManager(const std::wstring& i_libs_directory);
    ~DllManager();

    /// loads library and stores its handle
    void LoadLib(const std::wstring& i_filename);
    /// unloads all libraries; performs automatically in destructor
    void ReleaseLibraries();
  };