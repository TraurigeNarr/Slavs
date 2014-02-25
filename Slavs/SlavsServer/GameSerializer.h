#pragma once

#include <string>

class TiXmlElement;

namespace Slavs
  {
  class GameContext;

  class GameSerializer
    {
    private:
      GameContext& m_game_context;

    private:
      void Parse(TiXmlElement& i_root);

    public:
      GameSerializer(GameContext& i_game_context);
      ~GameSerializer();

      /// load game from file to i_game_context; need relative path from exe
      void Load(const std::string& i_game_name);

    };
  }