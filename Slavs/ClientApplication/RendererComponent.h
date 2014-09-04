#pragma once

#include <GameCore/IComponent.h>
#include <GameCore/Composer/IComponentSerializer.h>

namespace GameCore
  {
  class IObjectComposer;
  } // GameCore

namespace Ogre
  {
  class SceneNode;
  }

class ClientGameObject;

namespace ClientGame
  {

  class ModelController;
  class RendererComponentSerializer;

  class RendererComponent : public IComponent
    {
    friend RendererComponentSerializer;

    public:
      typedef RendererComponentSerializer TSerializer;

    private:
      Ogre::SceneNode*                 mp_scene_node;

    private:
      const GameCore::IObjectComposer& m_object_composer;

    public:
      RendererComponent(ClientGameObject* ip_owner, int i_component_id, const GameCore::IObjectComposer& i_composer);
      ~RendererComponent();

      virtual void	Tick() override;
      virtual bool	Probe() override;
    };

  class RendererComponentSerializer : public GameCore::IComponentSerializer
    {
    private:
      std::string m_file_name;
      std::string m_resource_group_name;
      std::string m_resource_type;

      ModelController& m_model_controller;

      std::array<float, 3> m_scale_factors;

    public:
      RendererComponentSerializer(int i_component_global_id, const GameCore::IObjectComposer& i_composer, ModelController& i_model_controller);
      ~RendererComponentSerializer();

      virtual void        Parse(const TiXmlElement& i_configuration_node) override;
      virtual void        ApplyTo(IComponent& i_component) const override;
      virtual IComponent* CreateComponent(GameObject* ip_object) const override;
    };

  } // ClientGame