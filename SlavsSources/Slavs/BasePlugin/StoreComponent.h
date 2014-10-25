#pragma once

#include "SlavsBasePluginAPI.h"

#include "IComponentSerializer.h"


#include <Game/GameResources.h>

#include <SlavsServer/Game/GameObject.h>
#include <SlavsServer/PluginSystem/IStore.h>
#include <SlavsServer/include/Types.h>

namespace BasePlugin
  {
  class StoreComponentSerializer;

  class StoreComponent : public Slavs::IStore
    {
    friend StoreComponentSerializer;

    public:
      typedef StoreComponentSerializer TSerializer;

    private:
      size_t                                              m_workers_number;
      std::vector<std::shared_ptr<GameResourceContainer>> m_resource_containers;
      const BaseObjectComposer&                           m_object_composer;

    public:
                    SLAVS_BASEPLUGIN_EXPORT StoreComponent(Slavs::TGameObject ih_owner, int i_component_id, const BaseObjectComposer& i_composer);
      virtual       SLAVS_BASEPLUGIN_EXPORT ~StoreComponent();
    
    // IComponent
    public:
      virtual void	  TickPerformed() override;
      virtual bool	  HandleMessage(const Telegram& msg) override;

      virtual void	  GetState(GameObjectState& i_state) const override;

      virtual bool	  Probe() override;

    // IStore
    public:
      virtual bool    SLAVS_BASEPLUGIN_EXPORT AddResource (GameResourceBox& i_resource_box) override;
      virtual bool    SLAVS_BASEPLUGIN_EXPORT AddResource (int i_type, size_t i_number) override;
      virtual size_t  SLAVS_BASEPLUGIN_EXPORT AddResourceForcely (int i_type, size_t i_number) override;
      virtual bool    SLAVS_BASEPLUGIN_EXPORT AddResourceContainer(std::shared_ptr<GameResourceContainer> ip_container) override;

      virtual size_t  SLAVS_BASEPLUGIN_EXPORT PeekResource (int i_type, size_t i_number) const override;
      virtual size_t  SLAVS_BASEPLUGIN_EXPORT GetResource (int i_type, size_t i_number) override;
      virtual size_t  SLAVS_BASEPLUGIN_EXPORT GetResourceForcely (int i_type, size_t i_number) override;

      virtual SLAVS_BASEPLUGIN_EXPORT const std::vector<std::shared_ptr<GameResourceContainer>>& GetResources() const override;
    };
  
  class StoreComponentSerializer : public IComponentSerializer
    {
    private:
      size_t                                            m_workers_number;
      std::vector<std::pair<GameResourceType, size_t>>  m_stored_resources;

    protected:
      void _FillResources(const TiXmlElement& i_resource_element);

    public:
      StoreComponentSerializer(int i_component_global_id, const BaseObjectComposer& i_composer);
      ~StoreComponentSerializer();

      virtual void        Parse(const TiXmlElement& i_configuration_node) override;
      virtual void        ApplyTo(IComponent& i_component) const override;
      virtual IComponent* CreateComponent(Slavs::GameObject* ip_object) const override;
    };

  } // BasePlugin