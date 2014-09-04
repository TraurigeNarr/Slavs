#include "stdafx.h"

#include "ClientComposer.h"

#include "ComponentTypes.h"
#include "RendererComponent.h"
#include "ModelController.h"
#include "ClientGameContext.h"

#include <Common/Utilities/XmlUtilities.h>

#include <GameCore/GameObject.h>
#include <GameCore/Composer/ObjectSettings.h>
#include <GameCore/Composer/IObjectComposer.h>
#include <GameCore/Composer/IComponentSerializer.h>

namespace
  {
  ComponentType _GetTypeFromString(const std::string& i_type)
    {
    if (i_type == "Renderer")
      return ComponentType::CT_RENDERER;
    return ComponentType::CT_NONE;
    }

  class ClientObjectSettings : public GameCore::ObjectSettings
    {
    private:
      ClientGame::ModelController&                  m_model_controller;

    private:
    virtual std::unique_ptr<GameCore::IComponentSerializer> _GetSerializerFromString(const std::string& i_type, const GameCore::IObjectComposer& i_composer) const override
      {
      if (i_type == "renderer")
        return std::unique_ptr<GameCore::IComponentSerializer>(
                                  new ClientGame::RendererComponent::TSerializer(i_composer.GetComponentGlobalID(static_cast<int>(ComponentType::CT_RENDERER)), 
                                                                                 GetComposer(), 
                                                                                 m_model_controller));
      return nullptr;
      }
    
    public:
      ClientObjectSettings (ClientGame::ModelController& i_model_controller, const GameCore::IObjectComposer& i_composer)
        : ObjectSettings(i_composer)
        , m_model_controller(i_model_controller)
        {        }    
    };
  } // namespace


namespace ClientGame
  {

  ClientComposer::ClientComposer (ClientGameContext& i_context)
    : m_context(i_context)
    {  }

  ClientComposer::~ClientComposer ()
    {

    }

  void ClientComposer::AddObjectDefinition (const std::string& i_object_name, int i_object_id)
    {
    m_definitions[i_object_name] = i_object_id;
    }

  void ClientComposer::Initialize (const std::string& i_configuration_file)
    {
    TiXmlDocument document;

    if(!XmlUtilities::LoadXmlDocument(i_configuration_file, document))
      throw std::invalid_argument("Can not load file");

    const TiXmlElement* p_root = document.RootElement();

    std::string object_name = "";

    const TiXmlElement* p_child = nullptr;
    const TiXmlElement* p_undefined_object = nullptr;
    int max_obj_type = 0;
    while ((p_child = XmlUtilities::IterateChildElements(p_root, p_child)))
      {
      object_name = p_child->Value();

      auto it = m_definitions.find(object_name);

      if (it != m_definitions.end())
        {
        mp_object_settings[it->second] = std::unique_ptr<GameCore::ObjectSettings>(new ClientObjectSettings(m_context.GetModelControlelr(), *this));
        mp_object_settings[it->second]->Initialize(it->second, *p_child);
        max_obj_type = std::max(max_obj_type, it->second);
        }
      else if (object_name == "Undefined")
        p_undefined_object = p_child;
      }

    if (p_undefined_object)
      {
      mp_object_settings[max_obj_type] = std::unique_ptr<GameCore::ObjectSettings>(new ClientObjectSettings(m_context.GetModelControlelr(), *this));
      mp_object_settings[max_obj_type]->Initialize(max_obj_type, *p_undefined_object);
      }

    }

  //////////////////////////////////////////////////////////////////////////
  // IObjectComposer

  void ClientComposer::ComposeObject (GameObject* ip_object)
    {
    auto settings_it = mp_object_settings.find(ip_object->GetType());
    assert (settings_it != mp_object_settings.end());

    settings_it->second->SetupObject(ip_object);
    }

  bool ClientComposer::Supports (int i_object_type)
    {
    auto it = mp_object_settings.find(i_object_type);
    return it != mp_object_settings.end();
    }

  void ClientComposer::DeclareSupportedTypes()
    {

    }

  bool ClientComposer::CheckContracts() const
    {
    return true;
    }

  int  ClientComposer::GetComponentGlobalID (int i_local_id) const
    {
    // because we have no place to register ids
    //    so we have local = global
    return i_local_id;
    }

  int  ClientComposer::GetObjectGlobalID (int i_local_id) const
    {
    return i_local_id;
    }

  } // ClientGame