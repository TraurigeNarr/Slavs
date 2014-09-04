#pragma once

#include <GameCore/GameContext.h>

class GameObjectState;
class Ogreframework;

namespace ClientGame
  {
  class ClientComposer;
  class ModelController;
  }

class ClientGameContext : public GameContext
  {
  private:
    std::unique_ptr<ClientGame::ClientComposer> mp_composer;
    std::unique_ptr<ClientGame::ModelController> mp_model_controller;

  private:
    void          _ApplyState(GameObjectState& i_state, GameObjectUniquePtr& ip_object);
    void          _AddObject(GameObjectState& i_state);

  protected:
    virtual void ReleaseContext() override;

  public:
    ClientGameContext(const std::string& i_context_name, OgreFramework& i_framework);
    virtual ~ClientGameContext();

    void          ApplyState(GameObjectState& i_state);
    void          AddDefinition(const std::pair<std::string, int>&& i_definition);

    ClientGame::ModelController& GetModelControlelr();

    void          Initialize();
  };

//////////////////////////////////////////////////////////////////////////

inline ClientGame::ModelController& ClientGameContext::GetModelControlelr()
  {
  return *mp_model_controller;
  }