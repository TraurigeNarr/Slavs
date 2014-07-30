#include "stdafx.h"

#include "ManufactureComponent.h"

#include "BaseObjectComposer.h"
#include "TypeNames.h"
#include "TypeEnumerations.h"

#include <Utilities/XmlUtilities.h>
#include <Game/GameObjectState.h>

#include <SlavsServer/include/IController.h>
#include <SlavsServer/include/Management/Goverment.h>
#include <SlavsServer/include/Management/IStoreSystem.h>
#include <SlavsServer/include/Management/IResourceManager.h>
#include <SlavsServer/include/Management/IEconomyManager.h>
#include <SlavsServer/include/ManufactureRequest.h>

#include <boost/lexical_cast.hpp>

//////////////////////////////////////////////////////////////////////////
namespace BasePlugin
  {

  //////////////////////////////////////////////////////////////////////////
  // ManufactureComponentSerializer

  ManufactureComponentSerializer::ManufactureComponentSerializer(int i_component_global_id, const BaseObjectComposer& i_composer)
    : IComponentSerializer(i_component_global_id, i_composer)
    {

    }

  ManufactureComponentSerializer::~ManufactureComponentSerializer()
    {

    }

  void ManufactureComponentSerializer::Parse(const TiXmlElement& i_configuration_node)
    {
    std::string elementName = "";

#ifdef _DEBUG
    elementName = XmlUtilities::GetStringAttribute(&i_configuration_node, "type", "");
    assert (elementName == Component_Manufacture);
    assert (i_configuration_node.FirstChildElement("worker"));
    assert (i_configuration_node.FirstChildElement("manufacture"));
    elementName = "";
#endif

    // skip worker information till there will be worker component
    if (true)
      {
      const TiXmlElement* p_worker  = i_configuration_node.FirstChildElement("worker");
      m_worker_type                 = m_object_composer.GetComponentGlobalID(BasePlugin::ComponentType::CT_HUMAN);
      }

    if (true)
      {
      const TiXmlElement* p_manufacture = i_configuration_node.FirstChildElement("manufacture");
      m_needed_workers = XmlUtilities::GetIntAttribute(p_manufacture->FirstChildElement(), "Calories",   10);
      const TiXmlElement* p_child_element = nullptr;
      while (p_child_element = XmlUtilities::IterateChildElements(p_manufacture, p_child_element))
        {
        elementName = p_child_element->Value();
        // food preferences
        if("needed_workers" == elementName)
          m_needed_workers = boost::lexical_cast<size_t>(p_child_element->FirstChild()->Value());
        else if ("initial_resources_mining" == elementName)
          m_initial_resources_mining = boost::lexical_cast<size_t>(p_child_element->FirstChild()->Value());
        else if ("operating_cycle" == elementName)
          m_operating_cycle = boost::lexical_cast<size_t>(p_child_element->FirstChild()->Value());
        else if ("mining_resource_type" == elementName)
          {
          std::string resource_type = p_child_element->FirstChild()->Value();
          m_mining_resource_type = static_cast<int>(GameResourceBox::DefineResType(resource_type));
          }
        else if ("initial_payment" == elementName)
          m_initial_payment = boost::lexical_cast<size_t>(p_child_element->FirstChild()->Value());
        }
      }
    
    }

  void ManufactureComponentSerializer::ApplyTo(IComponent& i_component) const 
    {
    assert (typeid(ManufactureComponent) == typeid(i_component));
    ManufactureComponent& manufacture = static_cast<ManufactureComponent&>(i_component);

    manufacture.m_needed_workers            = m_needed_workers;
    manufacture.m_initial_resources_mining  = m_initial_resources_mining;
    manufacture.m_operating_cycle           = m_operating_cycle;
    manufacture.m_initial_payment           = m_initial_payment;
    manufacture.m_mining_resource_type      = m_mining_resource_type;
    manufacture.m_worker_type               = m_worker_type;
    }

  IComponent* ManufactureComponentSerializer::CreateComponent(Slavs::GameObject* ip_object) const
    {
    ManufactureComponent* p_house = new ManufactureComponent(ip_object, m_component_global_id, m_object_composer);
    ApplyTo(*p_house);
    return p_house;
    }

  //////////////////////////////////////////////////////////////////////////
  // ManufactureComponent

  ManufactureComponent::ManufactureComponent(Slavs::TGameObject ip_owner, int i_component_id, const BaseObjectComposer& i_composer) 
    : IEmployer(ip_owner, i_component_id)
    , m_object_composer(i_composer)
    , mp_information(new EmployerInformation(this))
    , m_initial_payment(10)
    , m_initial_resources_mining(10)
    , m_mining_resource_type(0)
    , m_worker_type(1)
    , m_needed_workers(5)
    , m_operating_cycle(10)
    , m_state(ManufactureStates::MS_WAITING_FOR_WORKERS)
    , m_current_tick(0)
    {    
    if (ip_owner == nullptr)
      throw std::logic_error("Owner cannot be nullptr");
    ip_owner->GetController()->GetGoverment().GetEconomyManager()->RegisterEmployer(this);
    }

  ManufactureComponent::~ManufactureComponent()
    {    }

  //////////////////////////////////////////////////////////////////////////
  // IComponent

  void ManufactureComponent::TickPerformed()
    {
    if (m_state == ManufactureStates::MS_READY_TO_WORK)
      {
      if (!m_workers.empty())
        {
        m_state = ManufactureStates::MS_WORKING;
        m_current_tick = 0;
        }
      }
    else if (m_state == ManufactureStates::MS_WORKING)
      {
      ++m_current_tick;
      if (m_current_tick == m_operating_cycle)
        {
        size_t number = m_initial_resources_mining*m_workers.size();
        static_cast<Slavs::GameObject*>(mp_owner)->GetController()->GetGoverment().GetEconomyManager()->GetStoreSystem()->Add(std::make_pair(m_mining_resource_type, number));
        m_workers.empty();
        m_state = ManufactureStates::MS_WAITING_FOR_WORKERS;
        
        }
      }
    }

  bool ManufactureComponent::HandleMessage(const Telegram& msg)
    {
    return false;
    }

  void ManufactureComponent::GetState(GameObjectState& i_state) const
    {

    }

  bool ManufactureComponent::Probe()
    {
    return static_cast<Slavs::GameObject*>(mp_owner)->HasComponent(m_object_composer.GetComponentGlobalID(ComponentType::CT_MANUFACTURE)) 
        && static_cast<Slavs::GameObject*>(mp_owner)->HasComponent(m_object_composer.GetComponentGlobalID(ComponentType::CT_STATIC_COMPONENT));
    }

  //////////////////////////////////////////////////////////////////////////
  // IEmployer

  int ManufactureComponent::GetProducedResourceType() const
    {
    return m_mining_resource_type;
    }

  int ManufactureComponent::GetWorkerType() const
    {
    return m_worker_type;
    }

  bool ManufactureComponent::NeedWorkers() const
    {
    return m_state == ManufactureStates::MS_WAITING_FOR_WORKERS;
    }

  size_t ManufactureComponent::GetOperatingCycle() const
    {
    return m_operating_cycle;
    }

  bool ManufactureComponent::IsWorking() const
    {
    return m_state == ManufactureStates::MS_WORKING;
    }

  bool ManufactureComponent::IsSuitable(Slavs::HumanPtr ip_human)
    {
    return true;
    }

  bool ManufactureComponent::HireWorker(Slavs::HumanPtr ip_human)
    {
    if (m_state == ManufactureStates::MS_WORKING || m_state == ManufactureStates::MS_WAINTING_FOR_STORE || m_workers.size() == m_needed_workers)
      return false;
    
    m_state = ManufactureStates::MS_READY_TO_WORK;
    return m_workers.insert(ip_human).second;
    }

  void ManufactureComponent::StoreExpanded()
    {
    m_state = ManufactureStates::MS_WAITING_FOR_WORKERS;
    }

  const EmployerInformation& ManufactureComponent::GetInformation() const
    {
    return *mp_information;
    }

  //////////////////////////////////////////////////////////////////////////
  // IGoldKeeper

  void ManufactureComponent::ProcessEconomyEvent(EconomyEvent i_economy_event, void* ip_data)
    {

    }

  }