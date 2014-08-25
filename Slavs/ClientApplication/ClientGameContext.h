#pragma once

#include <GameCore/GameContext.h>

class GameObjectState;

class ClientGameContext : public GameContext
  {
  private:
    std::map<std::string, int>          m_definitions_map;

  private:
    void          _ApplyState(GameObjectState& i_state, GameObjectUniquePtr& ip_object);
    void          _AddObject(GameObjectState& i_state);

  protected:
    virtual void ReleaseContext() override;

  public:
    ClientGameContext(const std::string& i_context_name);
    virtual ~ClientGameContext();

    void          ApplyState(GameObjectState& i_state);
    void          AddDefinition(const std::pair<std::string, int>&& i_definition);

    void          Initialize();
  };