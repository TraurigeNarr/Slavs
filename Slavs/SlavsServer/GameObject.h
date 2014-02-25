#pragma once

#include "SlavsServerAPI.h"

#include <Game\IGameObject.h>

class IController;

namespace Slavs
  {
  class GameContext;

  class SLAVS_SERVER_EXPORT GameObject : public IGameObject
    {
    private:
      GameContext& m_context;
      IController* mp_owner;

    public:
      GameObject(GameContext& i_context, long i_id, int i_type, int i_selection_mask);
      ~GameObject();

      void          SetOwner(IController* ip_controller);
      bool          HasOwner() const;
      IController*  GetController();

      /// takes ownage of component
      void          AddComponent (IComponent* ip_component);
      /// validate state of components
      /// each component checks its state and that all
      /// dependencies are present in object
      bool          ProbeComponents ();

      //IGameObject overrides
    public:
      virtual void  TickPerformed() override;
      virtual void  Init() override;

      //use methods for transferring commands and notifications
//      virtual bool	ProcessCommand(const CommandData& cData) override;
//      virtual bool	HandleMessage(const Telegram& msg) override;
    };
  }