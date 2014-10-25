#include <Patterns/MessageDispatcher/MessageDispatcher.h>
#include <Patterns/MessageDispatcher/Event.h>

using ::testing::Return;
using ::testing::_;

//////////////////////////////////////////////////////////////////////////

namespace
  {

  struct TestEvent : Event
    {
    int TestData;
    TestEvent(int i_test_data = 0)
      : TestData(i_test_data)
      {      }
    };

  bool operator == (const TestEvent& i_first, const TestEvent& i_second)
    {
    return i_second.TestData == i_first.TestData;
    }

  class TestMessageHandler
    {
    public:
      MOCK_METHOD1(HandleTestMessage, void(const TestEvent&));
    };

  } // namespace



//////////////////////////////////////////////////////////////////////////

TEST(MessageDispatcher, TestMessageChain)
  {
  MessageDispatcher dispatcher;

  TestMessageHandler handler;

  dispatcher.RegisterHandler<TestMessageHandler, const TestEvent&> (handler, &TestMessageHandler::HandleTestMessage);

  TestEvent test_event(10);

  EXPECT_CALL(handler, HandleTestMessage(test_event)).Times(1);

  dispatcher.HandleMessage(test_event);
  }

TEST(MessageDispatcher, When_RemoveMessageHandler_MessageShouldNotPass)
  {
  MessageDispatcher dispatcher;

  TestMessageHandler handler;

  dispatcher.RegisterHandler<TestMessageHandler, const TestEvent&> (handler, &TestMessageHandler::HandleTestMessage);

  TestEvent test_event(10);

  EXPECT_CALL(handler, HandleTestMessage(_)).Times(0);

  dispatcher.UnregisterHandler<TestEvent> ();
  dispatcher.HandleMessage(test_event);
  }