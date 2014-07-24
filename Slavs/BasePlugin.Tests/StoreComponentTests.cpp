#include <BasePlugin/StoreComponent.h>
#include <BasePlugin/BaseObjectComposer.h>

#include <SlavsServer/Game/GameContext.h>
#include <SlavsServer/Game/GameObject.h>

#include <Server.Tests.Base/ControllerMock.h>

using namespace Slavs;
using namespace BasePlugin;
using ::testing::Return;
using ::testing::_;

//////////////////////////////////////////////////////////////////////////

TEST(StoreComponent, DefaultsTest)
  {
  BaseObjectComposer composer;
  Slavs::GameContext game_context;
  Slavs::GameObject game_object(game_context, 0, 0, 0);
  MockIController controller(0, game_context);
  game_object.SetOwner(&controller);

  StoreComponent store(&game_object, 0, composer);

  EXPECT_FALSE(store.AddResource(0, 1));
  EXPECT_FALSE(store.AddResource(GameResourceBox(GR_Tree, 1, 1, 1)));
  EXPECT_FALSE(store.PeekResource(0,1));
  EXPECT_EQ(0, store.GetResource(0, 1));
  }

TEST(StoreComponent, When_AddResourceSuccess_PeekResourceReturnsTrueAndGetResourceReturnNotZero)
  {
  BaseObjectComposer composer;
  Slavs::GameContext game_context;
  Slavs::GameObject game_object(game_context, 0, 0, 0);
  MockIController controller(0, game_context);
  game_object.SetOwner(&controller);

  StoreComponent store(&game_object, 0, composer);

  EXPECT_TRUE(store.AddResourceContainer(std::make_shared<GameResourceContainer>(GR_Stone, 100)));

  EXPECT_TRUE(store.AddResource(GR_Stone, 50));
  EXPECT_TRUE(store.PeekResource(GR_Stone, 50));
  EXPECT_EQ(25, store.GetResource(GR_Stone, 25));
  EXPECT_EQ(0, store.GetResource(GR_Stone, 26));
  EXPECT_EQ(25, store.GetResourceForcely(GR_Stone, 26));
  }

TEST(StoreComponent, When_AddGetResourceWhichStoreNotAccepts_ReturnsFailure)
  {
  BaseObjectComposer composer;
  Slavs::GameContext game_context;
  Slavs::GameObject game_object(game_context, 0, 0, 0);
  MockIController controller(0, game_context);
  game_object.SetOwner(&controller);

  StoreComponent store(&game_object, 0, composer);

  store.AddResourceContainer(std::make_shared<GameResourceContainer>(GR_Stone, 100));

  EXPECT_FALSE(store.AddResource(GR_Tree, 50));
  EXPECT_FALSE(store.PeekResource(GR_Tree, 50));
  EXPECT_EQ(0, store.GetResource(GR_Tree, 50));
  }

TEST(StoreComponent, When_AddMoreResourcesThanStoreCanAccept_ShouldReturnZero)
  {
  BaseObjectComposer composer;
  Slavs::GameContext game_context;
  Slavs::GameObject game_object(game_context, 0, 0, 0);
  MockIController controller(0, game_context);
  game_object.SetOwner(&controller);

  StoreComponent store(&game_object, 0, composer);
  store.AddResourceContainer(std::make_shared<GameResourceContainer>(GR_Stone, 50));

  EXPECT_FALSE(store.AddResource(GR_Stone, 51));
  EXPECT_EQ(50, store.AddResourceForcely(GR_Stone, 55));

  EXPECT_TRUE(store.GetResource(GR_Stone, 50));
  GameResourceBox box(GR_Stone, 100, 1, 1);
  EXPECT_FALSE(store.AddResource(box));
  EXPECT_EQ(50, box.GetNumber());
  }


TEST(StoreComponent, When_GetMoreResourcesThanStoreHas_ShouldReturnZero)
  {
  BaseObjectComposer composer;
  Slavs::GameContext game_context;
  Slavs::GameObject game_object(game_context, 0, 0, 0);
  MockIController controller(0, game_context);
  game_object.SetOwner(&controller);

  StoreComponent store(&game_object, 0, composer);
  store.AddResourceContainer(std::make_shared<GameResourceContainer>(GR_Stone, 100));

  EXPECT_TRUE(store.AddResource(GR_Stone, 50));
  EXPECT_FALSE(store.PeekResource(GR_Stone, 51));
  EXPECT_EQ(0, store.GetResource(GR_Stone, 51));
  }