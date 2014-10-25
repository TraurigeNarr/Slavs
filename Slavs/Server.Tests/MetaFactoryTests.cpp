#include <SlavsServer/PluginSystem/MetaFactory.h>

#include <SlavsServer/PluginSystem/IObjectComposer.h>

using namespace Slavs;
using ::testing::Return;
using ::testing::_;

namespace
  {
  
  class ComposerStub : public IObjectComposer
    {
    public:
      MOCK_METHOD1(ComposeObject, void(Slavs::GameObject*));
      MOCK_METHOD1(Supports, bool(int));
      MOCK_METHOD0(DeclareSupportedTypes, void(void));
      MOCK_CONST_METHOD0(CheckContracts,bool(void));
    };

  } // namespace

TEST(MetaFactory, When_RegisterType_Return_0)
  {
  const std::string object_dummy_type = "PluginName.PluginObject";
  MetaFactory factory;
  ASSERT_EQ(0, factory.RegisterType(object_dummy_type));
  ASSERT_EQ(0, factory.GetObjectID(object_dummy_type));
  }

TEST(MetaFactory, When_RegisterExistingType_Return_MinusOne)
  {
  const std::string object_dummy_type = "PluginName.PluginObject";
  MetaFactory factory;
  ASSERT_EQ(0, factory.RegisterType(object_dummy_type));
  ASSERT_EQ(-1, factory.RegisterType(object_dummy_type));  
  }

TEST(MetaFactory, When_SupportSelectedType_ReturnTrue)
  {
  const std::string object_dummy_type = "PluginName.PluginObject";
  MetaFactory factory;

  std::shared_ptr<ComposerStub> p_composer = std::make_shared<ComposerStub>();
  factory.RegisterObjectComposer(p_composer);

  EXPECT_CALL(*p_composer, Supports(0))
    .Times(2)
    .WillOnce(Return(true))
    .WillOnce(Return(false));

  factory.RegisterType(object_dummy_type);

  EXPECT_TRUE(factory.SupportObject(0));
  EXPECT_FALSE(factory.SupportObject(object_dummy_type));
  }