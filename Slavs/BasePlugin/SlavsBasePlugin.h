#pragma once

#include <PluginSystem/Plugin.h>
#include <string>

class SlavsBasePlugin : public Plugin
  {
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