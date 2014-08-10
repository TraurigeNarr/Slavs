#include <BasePlugin/MovementStrategyBase.h>

#include <SlavsServer/Game/GameContext.h>
#include <SlavsServer/Game/GameObject.h>


using namespace Slavs;
using namespace BasePlugin;
using ::testing::Return;
using ::testing::_;

//////////////////////////////////////////////////////////////////////////

TEST(MovementStrategyBase, SetTargetVector_Test)
  {
  MovementStrategyBase movement_strategy;

  Vector2D target_position(1.5, -1.23);
  movement_strategy.SetTarget(target_position);
  EXPECT_EQ(target_position, movement_strategy.GetTarget());
  }

TEST(MovementStrategyBase, SetTargetObject_Test)
  {
  MovementStrategyBase movement_strategy;

  Slavs::GameContext game_context;
  Slavs::GameObject target_object(game_context, 0, 0, 0);
  target_object.SetPosition(Vector2D(1.5, -1.23));

  movement_strategy.SetTarget(target_object);
  EXPECT_EQ(target_object.GetPosition(), movement_strategy.GetTarget());
  }

TEST(MovementStrategyBase, GetSteering_Return_TargetPosition)
  {
  MovementStrategyBase movement_strategy;

  Vector2D target_position(1.5, -1.23);
  movement_strategy.SetTarget(target_position);

  EXPECT_EQ(target_position, movement_strategy.GetSteering());
  Vector2D steering;
  movement_strategy.GetSteering(steering);
  EXPECT_EQ(target_position, steering);
  }