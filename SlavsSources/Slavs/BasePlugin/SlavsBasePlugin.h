#pragma once

#include <PluginSystem/Plugin.h>

#include <memory>
#include <string>

class BaseObjectComposer;

namespace SDK
	{
	namespace GameCore
		{
		class ICommandExecutor;
		}
	}

class IGameController;

class SlavsBasePlugin : public Plugin
  {
  std::shared_ptr<BaseObjectComposer> mh_object_composer;
	SDK::GameCore::ICommandExecutor*		mp_command_executor;
	IGameController*										mp_game_controller;

  public:
    SlavsBasePlugin ();
    virtual ~SlavsBasePlugin();

    virtual std::string GetName() const override;

    /// performs memory and resources allocations
    virtual void Install() override;
    /// performs logical initialization based on allocated resources
    virtual void Initialize() override;
    /// performs logical releasing 
    virtual void Release() override;
    /// performs memory and resources deletion
    virtual void Uninstall() override;
  };