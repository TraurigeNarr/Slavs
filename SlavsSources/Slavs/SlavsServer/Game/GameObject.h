#pragma once

#include "SlavsServerAPI.h"

#include <Common/Game/IGameObject.h>
#include <Common/Game/IMovable.h>

#include <memory>

class IController;

namespace Slavs
  {
  class GameContext;

  class GameObject : public IGameObject
    {
    private:
      GameContext& m_context;
      IController* mp_owner;

      std::vector<int> m_components_ids;

      Vector2D         m_position;

    public:
      SLAVS_SERVER_EXPORT GameObject(GameContext& i_context, long i_id, int i_type, int i_selection_mask);
      SLAVS_SERVER_EXPORT ~GameObject();

      SLAVS_SERVER_EXPORT void          SetOwner(IController* ip_controller);
      SLAVS_SERVER_EXPORT bool          HasOwner() const;
      /// check if component with global id "i_id" is in components collection
      SLAVS_SERVER_EXPORT bool          HasComponent(int i_id) const;
      SLAVS_SERVER_EXPORT IController*  GetController() const;

      /// takes ownage of component
			SLAVS_SERVER_EXPORT void          AddComponent(std::unique_ptr<IComponent> ih_component);
      /// validate state of components
      /// each component checks its state and that all
      /// dependencies are present in object
			SLAVS_SERVER_EXPORT bool          ProbeComponents();

      GameContext&  GetContext()
        {
        return m_context;
        }

    // IMovable
    public:
      SLAVS_SERVER_EXPORT virtual void                    SetPosition(const Vector2D& i_position);
      SLAVS_SERVER_EXPORT virtual Vector2D                GetPosition() const;
      SLAVS_SERVER_EXPORT virtual Vector2D&               AccessPosition();

      //IGameObject overrides
    public:
      SLAVS_SERVER_EXPORT virtual void                    TickPerformed() override;
      SLAVS_SERVER_EXPORT virtual void                    Init() override;
      SLAVS_SERVER_EXPORT virtual GameObjectState*		    GetState() const override;

      //use methods for transferring commands and notifications
//      virtual bool	ProcessCommand(const CommandData& cData) override;
//      virtual bool	HandleMessage(const Telegram& msg) override;
    };
  }