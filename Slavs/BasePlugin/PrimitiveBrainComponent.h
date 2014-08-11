#pragma once

#include "SlavsBasePluginAPI.h"

#include "IComponentSerializer.h"

#include <SlavsServer/PluginSystem/IHuman.h>

#include <SlavsServer/Game/GameObject.h>
#include <SlavsServer/include/Types.h>

class BaseObjectComposer;

namespace BasePlugin
  {

  class HumanComponent;

  class PrimitiveBrainComponentSerializer;

  class PrimitiveBrainComponent : public IComponent
    {
    friend PrimitiveBrainComponentSerializer;

    public:
      typedef PrimitiveBrainComponentSerializer TSerializer;

      HumanComponent* mp_human_component;

      // how many ticks human retires from work
      size_t          m_retire_period;
      size_t          m_period_without_work;
      bool            m_vaccation;

    public:
      SLAVS_BASEPLUGIN_EXPORT                         PrimitiveBrainComponent(Slavs::TGameObject ih_owner, int i_component_id, const BaseObjectComposer& i_object_composer);
      SLAVS_BASEPLUGIN_EXPORT                         ~PrimitiveBrainComponent();

      // IComponent
    public:
      virtual void	SLAVS_BASEPLUGIN_EXPORT           TickPerformed() override;
      virtual bool	SLAVS_BASEPLUGIN_EXPORT           HandleMessage(const Telegram& msg) override;

      virtual void	SLAVS_BASEPLUGIN_EXPORT           GetState(GameObjectState& i_state) const override;

      virtual bool	SLAVS_BASEPLUGIN_EXPORT           Probe() override;

    };

  class PrimitiveBrainComponentSerializer : public IComponentSerializer
    {
    private:
      size_t m_retire_period;

    public:
      PrimitiveBrainComponentSerializer(int i_component_global_id, const BaseObjectComposer& i_composer);
      ~PrimitiveBrainComponentSerializer();

      virtual void        Parse(const TiXmlElement& i_configuration_node) override;
      virtual void        ApplyTo(IComponent& i_component) const override;
      virtual IComponent* CreateComponent(Slavs::GameObject* ip_object) const override;
    };

  } // BasePlugin