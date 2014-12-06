#pragma once

#include "SlavsServerAPI.h"

#include "SlavsServer/CommandManager.h"

#include <boost/noncopyable.hpp>

#include <map>
#include <memory>
#include <vector>

//////////////////////////////////////////////////////////////////////////

class IObjectComposer;

namespace Slavs
  {
  class GameObject;
  }

/*
Base factory for other factories that are specific for 
types:
  1. GameObject - When object created all factories that supports this object type
    will include their pieces;
    Factory should be registered/unregister with 
      - RegisterObjectComposer
      - UnregisterComposer
  2. Managers - Factory for IEconomyManager, ISociualManager, etc..
  3. AI - to be implemented
All factories should register types that they support via:
  - RegisterType      - for objects like Humans, Trees, Manufactures, Space ships and so on
  - RegisterComponent - for components in objects (DynanicComponent, HumanComponent)
Or attach to use objects and components from other libraries

All registering operations should be in initialization step of Plugin
  - Plugin::Install
  - Plugin::Initialize
*/

class MetaFactory : boost::noncopyable
  {
  private:
    /// internal var for registration system
    int                       m_next_definitions_id;
    std::vector<std::string>  m_registered_types;

  public:
    typedef std::shared_ptr<IObjectComposer>  TObjectComposer;
    typedef std::vector<TObjectComposer>      TObjectComposers;
    typedef std::map<std::string, int>        TDefinitionsMap;
  private:
    TObjectComposers m_object_composers;

    /// this list should be transferred to client so it
    /// will be able to render all objects properly
    TDefinitionsMap          m_type_definitions;

		SDK::GameCore::CommandManager m_command_manager;
    
  public:
    SLAVS_SERVER_EXPORT MetaFactory();
    SLAVS_SERVER_EXPORT ~MetaFactory();

    /// adds to composers array
    void SLAVS_SERVER_EXPORT RegisterObjectComposer(TObjectComposer ih_composer);
    /// unregister if needed;
    /// use when plugin is uninstalled
    void SLAVS_SERVER_EXPORT UnregisterComposer(TObjectComposer ih_composer);

    /// register type and returns unique id that will be assigned to this type (for optimization)
    /// names must be unique; propose to use GUIDS or <PluginName>.<TypeName> {BasePlugin.HumanObject}
    /// use this function when your library is creator of this object
    int SLAVS_SERVER_EXPORT RegisterType(const std::string& i_type_name);
    /// register component and returns unique id that will be assigned to this type (for optimization)
    /// names must be unique; propose to use GUIDS or <PluginName>.<TypeName> {BasePlugin.HumanComponent}
    /// use this function when your library is creator of this component
    int SLAVS_SERVER_EXPORT RegisterComponent(const std::string& i_component_name);

    /// attaches to objects that was created by another library
    /// if there is no such objects
    int SLAVS_SERVER_EXPORT AttachToType(const std::string& i_type_name);
    int SLAVS_SERVER_EXPORT AttachToComponent(const std::string& i_component_name);

    /// validate all contracts between libraries
    /// 1. ObjectComposer - checks if all string ids have correct int id;
    ///                     if not throws runtime_error exception
    void SLAVS_SERVER_EXPORT CheckContracts();

    /// constructs object with all object composers which
    /// were registered on initialization state
    /// returns true if at least one composer supports object type
    bool ComposeObject(Slavs::GameObject* i_object);

    /// return true if at least one composer supports object type
    bool SLAVS_SERVER_EXPORT SupportObject(int i_type) const;
    /// return true if object with such type is registered
    bool SLAVS_SERVER_EXPORT SupportObject(const std::string& i_type) const;
    /// returns global id of object if such type name was registered
    /// in this factory
    /// -1 otherwise
    int  SLAVS_SERVER_EXPORT GetObjectID(const std::string& i_type) const;

    inline const TDefinitionsMap& GetDefinitions() const;

		SDK::GameCore::CommandManager& GetCommandManager();
  };

inline const MetaFactory::TDefinitionsMap& MetaFactory::GetDefinitions() const
  {
  return m_type_definitions;
  }

inline SDK::GameCore::CommandManager& MetaFactory::GetCommandManager()
	{
	return m_command_manager;
	}