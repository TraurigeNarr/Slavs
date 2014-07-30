#include <BasePlugin/ManufactureComponent.h>

#include <BasePlugin/BaseObjectComposer.h>

#include <SlavsServer/PluginSystem/IHuman.h>

#include <SlavsServer/Game/GameContext.h>
#include <SlavsServer/Game/GameObject.h>

#include <Server.Tests.Base/ControllerMock.h>
#include <Server.Tests.Base/HumanMock.h>
#include <Server.Tests.Base/GovermentMock.h>

using namespace Slavs;
using namespace BasePlugin;
using ::testing::Return;
using ::testing::_;

//////////////////////////////////////////////////////////////////////////

TEST(ManufactureComponent, DefaultsTest)
  {
  BaseObjectComposer composer;
  Slavs::GameContext game_context;
  Slavs::GameObject game_object(game_context, 0, 0, 0);
  MockIController controller(0, game_context);
  game_object.SetOwner(&controller);

  ManufactureComponent manufacture(&game_object, 0, composer);
  
  EXPECT_TRUE(manufacture.NeedWorkers());
  EXPECT_FALSE(manufacture.IsWorking());
  }

TEST(ManufactureComponent, WhenWorkersIsEnough_HireWorker_ShouldFails)
  {
  BaseObjectComposer composer;
  Slavs::GameContext game_context;
  Slavs::GameObject game_object(game_context, 0, 0, 0);
  MockIController controller(0, game_context);
  game_object.SetOwner(&controller);

  ManufactureComponent manufacture(&game_object, 0, composer);

  MockIHuman human_0(&game_object, 1);
  MockIHuman human_1(&game_object, 1);
  MockIHuman human_2(&game_object, 1);
  MockIHuman human_3(&game_object, 1);
  MockIHuman human_4(&game_object, 1);
  MockIHuman human_5(&game_object, 1);

  EXPECT_TRUE(manufacture.HireWorker(&human_0));
  EXPECT_TRUE(manufacture.HireWorker(&human_1));
  EXPECT_TRUE(manufacture.HireWorker(&human_2));
  EXPECT_TRUE(manufacture.HireWorker(&human_3));
  EXPECT_TRUE(manufacture.HireWorker(&human_4));
  EXPECT_FALSE(manufacture.HireWorker(&human_5));
  }

TEST(ManufactureComponent, WhenWorking_HireWorker_ShouldFails)
  {
  BaseObjectComposer composer;
  Slavs::GameContext game_context;
  Slavs::GameObject game_object(game_context, 0, 0, 0);
  MockIController controller(0, game_context);
  game_object.SetOwner(&controller);

  ManufactureComponent manufacture(&game_object, 0, composer);

  MockIHuman human(&game_object, 1);

  EXPECT_TRUE(manufacture.HireWorker(&human));
  // start working
  manufacture.TickPerformed();
  MockIHuman another_human(&game_object, 1);
  EXPECT_FALSE(manufacture.HireWorker(&another_human));
  }

TEST(ManufactureComponent, HireWorkerSecondTime_ShouldFails)
  {
  BaseObjectComposer composer;
  Slavs::GameContext game_context;
  Slavs::GameObject game_object(game_context, 0, 0, 0);
  MockIController controller(0, game_context);
  game_object.SetOwner(&controller);

  ManufactureComponent manufacture(&game_object, 0, composer);

  MockIHuman human(&game_object, 1);

  EXPECT_TRUE(manufacture.IsSuitable(&human));
  EXPECT_TRUE(manufacture.HireWorker(&human));
  EXPECT_FALSE(manufacture.HireWorker(&human));
  }

TEST(ManufactureComponent, HireWorker_StartWorkingOnNextTick)
  {
  BaseObjectComposer composer;
  Slavs::GameContext game_context;
  Slavs::GameObject game_object(game_context, 0, 0, 0);
  MockIController controller(0, game_context);
  game_object.SetOwner(&controller);

  ManufactureComponent manufacture(&game_object, 0, composer);

  MockIHuman human(&game_object, 1);

  EXPECT_TRUE(manufacture.IsSuitable(&human));
  EXPECT_TRUE(manufacture.HireWorker(&human));

  manufacture.TickPerformed();

  EXPECT_TRUE(manufacture.IsWorking());

  MockGoverment& goverment_mock = static_cast<MockGoverment&>(controller.GetGoverment());
  MockIEconomyManager* economy_manager = static_cast<MockIEconomyManager*>(goverment_mock.GetEconomyManager());
  MockIStoreSystem*    store_system = static_cast<MockIStoreSystem*>(economy_manager->GetStoreSystem());
  Slavs::ResourcePair res_pair(static_cast<GameResourceType>(0), 10);
  EXPECT_CALL(*store_system, Add(res_pair))
    .Times(1);

  for (size_t i = 0; i < manufacture.GetOperatingCycle(); ++i)
    manufacture.TickPerformed();

  EXPECT_FALSE(manufacture.IsWorking());
  }