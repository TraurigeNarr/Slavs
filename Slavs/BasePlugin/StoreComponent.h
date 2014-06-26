#pragma once

#include "IComponentSerializer.h"

#include <Game/IComponent.h>
#include <Game/GameResources.h>

#include <SlavsServer/Game/GameObject.h>
#include <SlavsServer/PluginSystem/IStore.h>
#include <SlavsServer/include/Types.h>

namespace BasePlugin
  {
  class StoreComponentSerializer;

  class StoreComponent : public IComponent
                       , public Slavs::IStore
    {
    friend StoreComponentSerializer;

    public:
      typedef StoreComponentSerializer TSerializer;

    private:
      size_t                                              m_workers_number;
      std::vector<std::shared_ptr<GameResourceContainer>> m_resource_containers;

    public:
      StoreComponent(Slavs::TGameObject ih_owner, int i_component_id);
      virtual       ~StoreComponent();
    
    // IComponent
    public:
      virtual void	  TickPerformed() override;
      virtual bool	  HandleMessage(const Telegram& msg) override;

      virtual void	  GetState(GameObjectState& i_state) const override;

      virtual bool	  Probe() override;

    // IStore
    public:
      virtual bool    AddResource (GameResourceBox& i_resource_box) override;
      virtual void    AddResource (int i_type, size_t i_number) override;

      virtual bool    PeekResource (int i_type, size_t i_number) override;
      virtual size_t  GetResource (int i_type, size_t i_number) override;
      virtual size_t  GetResourceForcely (int i_type, size_t i_number) override;

      virtual const std::vector<std::shared_ptr<GameResourceContainer>>& GetResources() const override;
    };

  class StoreComponentSerializer : public IComponentSerializer
    {
    private:
      size_t                                            m_workers_number;
      std::vector<std::pair<GameResourceType, size_t>>  m_stored_resources;

    protected:
      void _FillResources(const TiXmlElement& i_resource_element);

    public:
      StoreComponentSerializer(int i_component_id);
      ~StoreComponentSerializer();

      virtual void        Parse(const TiXmlElement& i_configuration_node) override;
      virtual void        ApplyTo(IComponent& i_component) const override;
      virtual IComponent* CreateComponent(Slavs::GameObject* ip_object) const override;
    };

  } // BasePlugin