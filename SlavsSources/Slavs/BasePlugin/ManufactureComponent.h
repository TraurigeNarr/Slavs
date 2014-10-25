#pragma once

#include "SlavsBasePluginAPI.h"

#include "IComponentSerializer.h"

#include <SlavsServer/PluginSystem/IEmployer.h>

#include <SlavsServer/Game/GameObject.h>
#include <SlavsServer/include/Types.h>

namespace BasePlugin
  {

  class ManufactureComponentSerializer;

  class ManufactureComponent : public Slavs::IEmployer
    {
    friend ManufactureComponentSerializer;

    public:
      typedef ManufactureComponentSerializer TSerializer;

    private:
      const BaseObjectComposer&                   m_object_composer;
      std::unique_ptr<EmployerInformation>        mp_information;
      size_t                                      m_needed_workers;
      size_t                                      m_initial_resources_mining;
      size_t                                      m_operating_cycle;
      int                                         m_mining_resource_type;
      int                                         m_worker_type;
      size_t                                      m_initial_payment;

      enum class ManufactureStates
        {
        MS_WAITING_FOR_WORKERS,
        MS_WAINTING_FOR_STORE,
        MS_READY_TO_WORK,
        MS_WORKING
        };

      ManufactureStates                           m_state;

      std::set<Slavs::HumanPtr>                   m_workers;

      size_t                                      m_current_tick;

    public:
      SLAVS_BASEPLUGIN_EXPORT                 ManufactureComponent(Slavs::TGameObject ip_owner, int i_component_id, const BaseObjectComposer& i_composer);
      virtual       SLAVS_BASEPLUGIN_EXPORT   ~ManufactureComponent();

      // IComponent
    public:
      virtual void	SLAVS_BASEPLUGIN_EXPORT         TickPerformed() override;
      virtual bool	SLAVS_BASEPLUGIN_EXPORT         HandleMessage(const Telegram& msg) override;

      virtual void	SLAVS_BASEPLUGIN_EXPORT         GetState(GameObjectState& i_state) const override;
      virtual bool	SLAVS_BASEPLUGIN_EXPORT         Probe() override;

      // IEmployer
    public:
      virtual int     SLAVS_BASEPLUGIN_EXPORT       GetProducedResourceType() const override;
      virtual int     SLAVS_BASEPLUGIN_EXPORT       GetWorkerType() const override;
      virtual bool    SLAVS_BASEPLUGIN_EXPORT       NeedWorkers() const override;
      virtual bool    SLAVS_BASEPLUGIN_EXPORT       IsWorking() const override;
      virtual size_t  SLAVS_BASEPLUGIN_EXPORT       GetOperatingCycle() const override;

      virtual bool    SLAVS_BASEPLUGIN_EXPORT       IsSuitable(Slavs::HumanPtr ip_human) override;
      virtual bool    SLAVS_BASEPLUGIN_EXPORT       HireWorker(Slavs::HumanPtr ip_human) override;
      virtual void    SLAVS_BASEPLUGIN_EXPORT       StoreExpanded() override;

      virtual SLAVS_BASEPLUGIN_EXPORT const EmployerInformation& GetInformation() const override;

      // IGoldKeeper
    public:
      virtual void ProcessEconomyEvent(EconomyEvent i_event, void* ip_data = nullptr) override;
    };

  class ManufactureComponentSerializer : public IComponentSerializer
    {
    private:
      // worker - skip this till worker component will be
      size_t m_needed_workers;
      size_t m_initial_resources_mining;
      size_t m_operating_cycle;
      int    m_mining_resource_type;
      int    m_worker_type;
      size_t m_initial_payment;

    public:
      ManufactureComponentSerializer(int i_component_id, const BaseObjectComposer& i_composer);
      ~ManufactureComponentSerializer();

      virtual void        Parse(const TiXmlElement& i_configuration_node) override;
      virtual void        ApplyTo(IComponent& i_component) const override;
      virtual IComponent* CreateComponent(Slavs::GameObject* ip_object) const override;
    };

  } // BasePlugin