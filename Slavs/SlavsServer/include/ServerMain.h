#ifndef ServerMain_h
#define ServerMain_h

#include "SlavsServerAPI.h"

#include "PluginSystem/MetaFactory.h"
#include <Patterns/StateMachine.h>

#include <memory>
#include <set>

//////////////////////////////////////////////////////////////////////////

class DllManager;
class Plugin;

namespace net
  {
  class Connection;
  }

class SLAVS_SERVER_EXPORT ServerMain
{
private:
  static std::shared_ptr<ServerMain> mh_instance;

  ServerMain(const ServerMain& i_other);
  ServerMain& operator = (const ServerMain& i_other);

private:
  typedef std::set<Plugin*> TPlugins;

private:
  std::unique_ptr<net::Connection> mh_server_connection;
  bool m_bWorking;

  StateMachine<ServerMain, long> *m_pFSM;

  std::unique_ptr<DllManager> mh_dll_manager;
  MetaFactory m_meta_factory;
  TPlugins m_plugins;

public:
	ServerMain();
	~ServerMain();

  __declspec(deprecated)
	bool Initialize();
	void Shutdown();

	void Update(long elapsedTime);

	bool Working(){ return m_bWorking; }

	net::Connection* GetConnection() const;
	StateMachine<ServerMain, long>* GetFSM() const { return m_pFSM; }
	bool FromGame;

//////////////////////////////////////////////////////////////////////////
public:
  /// Starts server
  /// @param i_configuration_file - absolute path to file with configurations
  /// in file should be present information about 
  ///   1. Port where to start
  ///   2. Addresses of connected players
  ///   3. Plugins which must be loaded
  ///   4. Relative or absolute path to game name
  bool                Start(const std::string& i_configuration_file);
  MetaFactory&        GetMetaFactory();
  DllManager&         GetDllManager();

  void                RegisterPlugin(Plugin* ip_plugin);
  void                UnregisterPlugin(Plugin* ip_plugin);

//////////////////////////////////////////////////////////////////////////
  static ServerMain&  GetInstance();

private:
  
};

#endif