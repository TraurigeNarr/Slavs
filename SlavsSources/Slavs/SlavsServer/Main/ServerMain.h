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

class ServerMain
  {
public:
    typedef StateMachine<ServerMain, long>  TServerFSM;
    typedef std::unique_ptr<TServerFSM>     THServerFSM;
    typedef std::set<Plugin*>               TPlugins;
private:
  static std::unique_ptr<ServerMain> mh_instance;

  ServerMain(const ServerMain& i_other);
  ServerMain& operator = (const ServerMain& i_other);

private:
  StateMachine<ServerMain, long> *m_pFSM;

  THServerFSM                       mh_state_machine;
  std::unique_ptr<net::Connection>  mh_server_connection;
  std::unique_ptr<DllManager>       mh_dll_manager;
  MetaFactory                       m_meta_factory;
  TPlugins                          m_plugins;
  bool                              m_bWorking;

public:
	SLAVS_SERVER_EXPORT ServerMain();
	SLAVS_SERVER_EXPORT ~ServerMain();

  __declspec(deprecated)
		SLAVS_SERVER_EXPORT bool Initialize();

	SLAVS_SERVER_EXPORT void Update(long elapsedTime);

	bool Working(){ return m_bWorking; }

	SLAVS_SERVER_EXPORT net::Connection* GetConnection() const;
	StateMachine<ServerMain, long>* GetFSM() const { throw std::runtime_error("Deprecated"); return m_pFSM; }
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
	SLAVS_SERVER_EXPORT bool                Start(const std::string& i_configuration_file);
  /// Sets internal flag so Working will return false
  ///   Shutdown must be call after stop
  SLAVS_SERVER_EXPORT void                Stop();
  SLAVS_SERVER_EXPORT void                Shutdown();

  SLAVS_SERVER_EXPORT MetaFactory&        GetMetaFactory();
  SLAVS_SERVER_EXPORT DllManager&         GetDllManager();

  SLAVS_SERVER_EXPORT void                RegisterPlugin(Plugin* ip_plugin);
  SLAVS_SERVER_EXPORT void                UnregisterPlugin(Plugin* ip_plugin);
  SLAVS_SERVER_EXPORT TServerFSM&         GetStateMachine();
//////////////////////////////////////////////////////////////////////////
	SLAVS_SERVER_EXPORT static ServerMain&  GetInstance();
  };

#endif