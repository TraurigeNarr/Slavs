#include "stdafx.h"

#include "PrimitiveBrainComponent.h"

#include "BaseObjectComposer.h"
#include "HumanComponent.h"
#include "TypeNames.h"
#include "TypeEnumerations.h"

#include <Utilities/XmlUtilities.h>
#include <Game/GameObjectState.h>

#include <SlavsServer/include/IController.h>
#include <SlavsServer/include/Management/Goverment.h>
#include <SlavsServer/include/Management/IEconomyManager.h>


namespace BasePlugin
  {
  //////////////////////////////////////////////////////////////////////////
  // Human Component Serializer 
  PrimitiveBrainComponentSerializer::PrimitiveBrainComponentSerializer(int i_component_global_id, const BaseObjectComposer& i_composer)
    : IComponentSerializer(i_component_global_id, i_composer)
    , m_retire_period(0)
    {

    }

  PrimitiveBrainComponentSerializer::~PrimitiveBrainComponentSerializer()
    {

    }

  void PrimitiveBrainComponentSerializer::Parse(const TiXmlElement& i_configuration_node)
    {
    std::string elementName = "";

#ifdef _DEBUG
    elementName = XmlUtilities::GetStringAttribute(&i_configuration_node, "type", "");
    assert (elementName == Component_PrimitiveBrain);
    elementName = "";
#endif

    m_retire_period = XmlUtilities::GetIntAttribute(&i_configuration_node, "retire_period", 0);
    }

  void PrimitiveBrainComponentSerializer::ApplyTo(IComponent& i_component) const 
    {
    assert (typeid(PrimitiveBrainComponent) == typeid(i_component));
    PrimitiveBrainComponent& primitive_brain = static_cast<PrimitiveBrainComponent&>(i_component);

    primitive_brain.m_retire_period = m_retire_period;
    }

  IComponent* PrimitiveBrainComponentSerializer::CreateComponent(Slavs::GameObject* ip_object) const
    {
    PrimitiveBrainComponent* p_brain = new PrimitiveBrainComponent(ip_object, m_component_global_id, m_object_composer);
    ApplyTo(*p_brain);
    return p_brain;
    }

  //////////////////////////////////////////////////////////////////////////
  // Human Component

  PrimitiveBrainComponent::PrimitiveBrainComponent(Slavs::TGameObject ih_owner, int i_component_id, const BaseObjectComposer& i_composer)
    : IComponent(ih_owner, i_component_id)
    , mp_human_component(nullptr)
    , m_vaccation(false)
    , m_retire_period(0)
    , m_period_without_work(0)
    {

    }

  PrimitiveBrainComponent::~PrimitiveBrainComponent()
    {

    }

  void PrimitiveBrainComponent::TickPerformed()
    {
    if (mp_human_component->HasWork())
      return;

    // when human obtain work and when retired
    if (m_vaccation)
      {
      ++m_period_without_work;
      if (m_period_without_work >= m_retire_period)
        m_vaccation = false;
      else
        return;
      }

    Slavs::TEmployersInformation available_employers;
    static_cast<Slavs::GameObject*>(GetOwner())->GetController()->GetGoverment().GetEconomyManager()->GetAvailableEmployers(available_employers);

    for (const EmployerInformation* p_employer_information : available_employers)
      {
      if (p_employer_information->IsActive() && p_employer_information->GetOwner()->IsSuitable(mp_human_component) && p_employer_information->GetOwner()->HireWorker(mp_human_component))
        {
        mp_human_component->HandleMessage(Telegram(0, 0, 0, 1));
        m_period_without_work = 0;
        m_vaccation = true;
        }
      }
    }

  bool PrimitiveBrainComponent::HandleMessage(const Telegram& msg)
    {
    return false;
    }

  void PrimitiveBrainComponent::GetState(GameObjectState& i_state) const
    {

    }

  bool PrimitiveBrainComponent::Probe()
    {
    mp_human_component = mp_owner->GetComponent<HumanComponent>();
    return mp_human_component != nullptr
      && mp_owner->GetComponent<PrimitiveBrainComponent>() == this;
    }
  } // BasePlugin