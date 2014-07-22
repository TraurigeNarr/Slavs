#include <BasePlugin/HumanComponent.h>

#include <SlavsServer/PluginSystem/IHouse.h>

#include <SlavsServer/Game/GameContext.h>
#include <SlavsServer/Game/GameObject.h>

using namespace Slavs;
using namespace BasePlugin;
using ::testing::Return;
using ::testing::_;

namespace 
  {
  class MockIHouse : public Slavs::IHouse 
    {
    public:
      MockIHouse(Slavs::TGameObject ih_owner, int i_component_id)
        : IHouse(ih_owner, i_component_id)
        {        }
      MOCK_METHOD1(HandleMessage, 
        bool(const Telegram& i_message));
      MOCK_CONST_METHOD0(GetPopulation,
        size_t());
      MOCK_CONST_METHOD0(GetUnemployedPopulation,
        size_t());
      MOCK_CONST_METHOD1(GetUnemployedPopulation,
        void(Humans& o_unemployed));
      MOCK_CONST_METHOD0(GetFreePlaces,
        size_t());
      MOCK_METHOD1(AddInhabitant,
        bool(HumanPtr ip_inhabitant));
      MOCK_METHOD1(RemoveInhabitant,
        void(HumanPtr ip_inhabitant));
      MOCK_METHOD0(HumanStateChanged,
        void());
    };
  }  // namespace

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