#include "stdafx.h"

#include "RendererComponent.h"

#include "ClientGameObject.h"

#include "ComponentTypes.h"

#include "ModelController.h"
#include "OgreMaxModel.h"

#include <GameCore/Composer/IObjectComposer.h>

#include <Common/Utilities/XmlUtilities.h>
#include <Common/Utilities/FileUtilities.h>

#include <Ogre/OgreSceneNode.h>
#include <OgreAny.h>

using namespace GameCore;

namespace ClientGame
  {

  //////////////////////////////////////////////////////////////////////////
  // Static Component Serializer 
  RendererComponentSerializer::RendererComponentSerializer(int i_component_global_id, const IObjectComposer& i_composer, ModelController& i_model_controller)
    : IComponentSerializer(i_component_global_id, i_composer)
    , m_model_controller(i_model_controller)
    {

    }

  RendererComponentSerializer::~RendererComponentSerializer()
    {

    }

  void RendererComponentSerializer::Parse(const TiXmlElement& i_configuration_node)
    {
#ifdef _DEBUG
    std::string elementName = "";
    elementName = XmlUtilities::GetStringAttribute(&i_configuration_node, "type", "");
    assert (elementName == "renderer");
#endif
    const TiXmlElement* p_model_element = i_configuration_node.FirstChildElement("model");
    std::string location    = XmlUtilities::GetStringAttribute(p_model_element, "location", "");
    std::string file        = XmlUtilities::GetStringAttribute(p_model_element, "file", "");
    m_file_name             = FileUtilities::JoinPath(FileUtilities::GetApplicationDirectory(), location);
    m_file_name             = FileUtilities::JoinPath(m_file_name, file);
    m_resource_group_name   = XmlUtilities::GetStringAttribute(p_model_element, "group", "");

    if (const TiXmlElement* p_scale = i_configuration_node.FirstChildElement("scale"))
      {
      m_scale_factors[0] = XmlUtilities::GetRealAttribute(p_scale, "x", 1.f);
      m_scale_factors[1] = XmlUtilities::GetRealAttribute(p_scale, "y", 1.f);
      m_scale_factors[2] = XmlUtilities::GetRealAttribute(p_scale, "z", 1.f);
      }
    }

  void RendererComponentSerializer::ApplyTo(IComponent& i_component) const
    {
    assert (typeid(RendererComponent) == typeid(i_component));
    RendererComponent& renderer = static_cast<RendererComponent&>(i_component);
    
    auto p_model = m_model_controller.GetModel(m_file_name, m_resource_group_name);
    if (p_model != nullptr)
      {
      OgreMax::OgreMaxModel::InstanceOptions options;
      options |= OgreMax::OgreMaxModel::NO_ANIMATION_STATES;
      renderer.mp_scene_node = p_model->CreateInstance(
        m_model_controller.GetOgreFramework().GetSceneManager(),
        0,                                                                            // query mask
        nullptr,                                                                      // callback
        options,                                                                      // options
        m_model_controller.GetOgreFramework().GetSceneManager()->getRootSceneNode(),  // parent
        m_resource_group_name,                                                        // resource group name
        nullptr                                                                       // node
        );
      renderer.mp_scene_node->setInheritScale(true);
      renderer.mp_scene_node->setVisible(true);
      renderer.mp_scene_node->getUserObjectBindings().setUserAny(Ogre::Any(renderer.GetOwner()));
      }
    }

  IComponent* RendererComponentSerializer::CreateComponent(GameObject* ip_object) const
    {
    RendererComponent* p_static = new RendererComponent(static_cast<ClientGameObject*>(ip_object), m_component_global_id, m_object_composer);
    ApplyTo(*p_static);
    return p_static;
    }


  //////////////////////////////////////////////////////////////////////////
  // Static Component

  RendererComponent::RendererComponent(ClientGameObject* ip_owner, int i_component_id, const IObjectComposer& i_composer)
    : IComponent(i_component_id, ip_owner)
    , mp_scene_node(nullptr)
    , m_object_composer(i_composer)
    {

    }

  RendererComponent::~RendererComponent()
    {

    }

  void RendererComponent::Tick()
    {
    if (!mp_owner->HasChanges())
      return;

    mp_scene_node->setPosition(mp_owner->GetPosition()[0], 0, mp_owner->GetPosition()[1]);
    mp_owner->Validate();
    }

  bool RendererComponent::Probe()
    {
    return mp_owner->HasComponent(m_object_composer.GetComponentGlobalID(static_cast<int>(ComponentType::CT_RENDERER)));
    }

  } // ClientGame