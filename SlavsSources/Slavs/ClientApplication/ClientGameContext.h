#pragma once

#include <GameCore/GameContext.h>

class GameObjectState;
class Ogreframework;

class Vector2D;

namespace ClientGame
  {
  class ClientComposer;
  class ModelController;
  class SceneController;
  }

class ClientGameContext : public GameContext
  {
  private:
    std::unique_ptr<ClientGame::ClientComposer>   mp_composer;
    std::unique_ptr<ClientGame::ModelController>  mp_model_controller;

    bool                                          m_create_unknown_objects;

    std::unique_ptr<ClientGame::SceneController>  mp_scene_controller;

  private:
		void          _ApplyState(GameObjectState& i_state, GameObject* ip_object);
    void          _AddObject(GameObjectState& i_state);

  protected:
    virtual void ReleaseContext() override;

  public:
    ClientGameContext(const std::string& i_context_name, OgreFramework& i_framework);
    virtual ~ClientGameContext();

    void                          CreateUnknownObjects (bool i_create);

    void                          ApplyState(GameObjectState& i_state);
    void                          AddDefinition(const std::pair<std::string, int>&& i_definition);
    
    void                          Initialize();

    virtual void                  TickPerformed() override;

		std::vector<long>							GetObjcetsInBox(const Vector2D& i_top_left, const Vector2D& i_bottom_right) const;

    ClientGame::ModelController&  GetModelControlelr();    
    ClientGame::SceneController&  GetSceneController();
  };

//////////////////////////////////////////////////////////////////////////

inline ClientGame::ModelController& ClientGameContext::GetModelControlelr()
  {
  return *mp_model_controller;
  }

inline ClientGame::SceneController& ClientGameContext::GetSceneController()
  {
  return *mp_scene_controller;
  }

inline void ClientGameContext::CreateUnknownObjects(bool i_create)
  {
  m_create_unknown_objects = i_create;
  }