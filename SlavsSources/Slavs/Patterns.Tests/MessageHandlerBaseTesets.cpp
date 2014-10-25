#include <Patterns/MessageDispatcher/MessageHandlerBase.h>
#include <Patterns/MessageDispatcher/Event.h>

using ::testing::_;

namespace
  {

  class TestHandler : public MessageHandlerBase
    {
    public:
      MOCK_METHOD1(HandleMessage, void (const Event& i_event));
    };

  }

TEST(MessageHandlerBase, When_ExecuteHandler_Call_HandleMessage)
  {
  TestHandler handler;

  EXPECT_CALL(handler, HandleMessage(_));

  handler.ExecuteHandler(Event());
  }