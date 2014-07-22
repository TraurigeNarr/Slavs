#include <BasePlugin/HumanComponent.h>

#include <SlavsServer/PluginSystem/IHouse.h>

#include <SlavsServer/Game/GameContext.h>
#include <SlavsServer/Game/GameObject.h>

#include <Server.Tests.Base/HouseMock.h>

using namespace Slavs;
using namespace BasePlugin;
using ::testing::Return;
using ::testing::_;

TEST(HumanComponent, HasWork_ReturnFalseByDefault)
  {
  Slavs::GameContext game_context;
  Slavs::GameObject game_object(game_context, 0, 0, 0);

  HumanComponent human(&game_object, 0);
  ASSERT_FALSE(human.HasWork());
  }

TEST(HumanComponent, GetHome_ReturnNullByDefault)
  {
  Slavs::GameContext game_context;
  Slavs::GameObject game_object(game_context, 0, 0, 0);
  HumanComponent human(&game_object, 0);
  ASSERT_EQ(nullptr, human.GetHome());
  }

TEST(HumanComponent, GetHome_ReturnValidPointer)
  {
  Slavs::GameContext game_context;
  Slavs::GameObject game_object(game_context, 0, 0, 0);
  HumanComponent human(&game_object, 0);
  MockIHouse house(&game_object, 0);
  human.SetHome(&house);
  ASSERT_EQ(&house, human.GetHome());
  }