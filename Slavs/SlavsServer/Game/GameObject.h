#pragma once

#include "SlavsServerAPI.h"

#include <Game\IGameObject.h>
#include <Game\IMovable.h>

#include <memory>

class IController;

namespace Slavs
  {
  class GameContext;

  class SLAVS_SERVER_EXPORT GameObject 
    : public IGameObject
    , public IMovable
    {
    private:
      GameContext& m_context;
      IController* mp_owner;

      std::vector<int> m_components_ids;

      Vector2D         m_position;

    public:
      GameObject(GameContext& i_context, long i_id, int i_type, int i_selection_mask);
      ~GameObject();

      void          SetOwner(IController* ip_controller);
      bool          HasOwner() const;
      /// check if component with global id "i_id" is in components collection
      bool          HasComponent(int i_id) const;
      IController*  GetController();

      /// takes ownage of component
      void          AddComponent (std::unique_ptr<IComponent> ih_component);
      /// validate state of components
      /// each component checks its state and that all
      /// dependencies are present in object
      bool          ProbeComponents ();

    // IMovable
    public:
      virtual void                    SetPosition(const Vector2D& i_position);
      virtual Vector2D                GetPosition() const;
      virtual Vector2D&               AccessPosition();

      //IGameObject overrides
    public:
      virtual void                    TickPerformed() override;
      virtual void                    Init() override;
      virtual GameObjectState*		    GetState() const override;

      //use methods for transferring commands and notifications
//      virtual bool	ProcessCommand(const CommandData& cData) override;
//      virtual bool	HandleMessage(const Telegram& msg) override;
    };
  }