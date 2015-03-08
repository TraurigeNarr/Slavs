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
      void ParseGameDocument(TiXmlElement& i_root);
			void ParseObjects(const TiXmlElement& i_root);
			void ParseSpawns(const TiXmlElement& i_root);
      void ParseConfigurationDocument(TiXmlElement& i_root);
    public:
      GameSerializer(GameContext& i_game_context);
      ~GameSerializer();

      /// load game from file to i_game_context; need relative path from exe
      void LoadGame(const std::string& i_game_name);
      void LoadConfigurations(const std::string& i_configurations_path);
    };
  }