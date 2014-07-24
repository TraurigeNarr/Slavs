#include <BasePlugin/HouseComponent.h>
#include <BasePlugin/BaseObjectComposer.h>

#include <SlavsServer/PluginSystem/IHuman.h>

#include <SlavsServer/Game/GameContext.h>
#include <SlavsServer/Game/GameObject.h>

#include <Server.Tests.Base/HumanMock.h>
#include <Server.Tests.Base/ControllerMock.h>

using namespace Slavs;
using namespace BasePlugin;
using ::testing::Return;
using ::testing::_;

//////////////////////////////////////////////////////////////////////////

TEST(HouseComponent, DefaultsTest)
  {
  BaseObjectComposer composer;
  Slavs::GameContext game_context;
  Slavs::GameObject game_object(game_context, 0, 0, 0);
  MockIController controller(0, game_context);
  game_object.SetOwner(&controller);
  
  HouseComponent house(&game_object, 0, composer);

  EXPECT_EQ(0, house.GetUnemployedPopulation());
  EXPECT_EQ(0, house.GetPopulation());
  EXPECT_EQ(0, house.GetFreePlaces());
  }

TEST(HouseComponent, WhenNoFreePlaces_ShouldReturnFalse)
  {
  BaseObjectComposer composer;
  Slavs::GameContext game_context;
  Slavs::GameObject game_object(game_context, 0, 0, 0);
  MockIController controller(0, game_context);
  game_object.SetOwner(&controller);

  HouseComponent house(&game_object, 0, composer);
  MockIHuman human(&game_object, 1);

  ASSERT_EQ(0, house.GetFreePlaces());
  ASSERT_FALSE(house.AddInhabitant(&human));
  }

TEST(HouseComponent, WhenAddInhabitant_ShouldReturnTrue_AndChangeHouseState)
  {
  BaseObjectComposer composer;
  Slavs::GameContext game_context;
  Slavs::GameObject game_object(game_context, 0, 0, 0);
  MockIController controller(0, game_context);
  game_object.SetOwner(&controller);

  HouseComponent house(&game_object, 0, composer);
  MockIHuman human(&game_object, 1);

  house.IncreaseMaximumInhabitatnsBy(1);

  ASSERT_EQ(1, house.GetFreePlaces());
  ASSERT_TRUE(house.AddInhabitant(&human));

  ASSERT_EQ(0, house.GetFreePlaces());
  EXPECT_EQ(1, house.GetPopulation());
  }

TEST(HouseComponent, When_HumanHasWork_GetUnemployed_ShouldCall_HasWork)
  {
  BaseObjectComposer composer;
  Slavs::GameContext game_context;
  Slavs::GameObject game_object(game_context, 0, 0, 0);
  MockIController controller(0, game_context);
  game_object.SetOwner(&controller);

  HouseComponent house(&game_object, 0, composer);
  MockIHuman human(&game_object, 1);

  house.IncreaseMaximumInhabitatnsBy(1);

  ASSERT_EQ(1, house.GetFreePlaces());
  ASSERT_TRUE(house.AddInhabitant(&human));

  EXPECT_CALL(human, HasWork())
    .Times(1)
    .WillOnce(Return(true));

  Slavs::Humans unemployed;
  house.GetUnemployedPopulation(unemployed);

  EXPECT_EQ(0, unemployed.size());
  }

TEST(HouseComponent, When_HumanHasNoWork_GetUnemployed_ShouldCall_HasWork)
  {
  BaseObjectComposer composer;
  Slavs::GameContext game_context;
  Slavs::GameObject game_object(game_context, 0, 0, 0);
  MockIController controller(0, game_context);
  game_object.SetOwner(&controller);

  HouseComponent house(&game_object, 0, composer);
  MockIHuman human(&game_object, 1);

  house.IncreaseMaximumInhabitatnsBy(1);

  ASSERT_EQ(1, house.GetFreePlaces());
  ASSERT_TRUE(house.AddInhabitant(&human));

  EXPECT_CALL(human, HasWork())
    .Times(1)
    .WillOnce(Return(true));

  Slavs::Humans unemployed;
  house.GetUnemployedPopulation(unemployed);

  EXPECT_EQ(0, unemployed.size());
  }