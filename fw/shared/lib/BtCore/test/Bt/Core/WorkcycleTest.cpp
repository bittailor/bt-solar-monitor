//*************************************************************************************************
//
//  BITTAILOR.CH
//
//*************************************************************************************************

#include <gtest/gtest.h>

#include "Bt/Core/Workcycle.h"
#include "Bt/Core/RunnableGMock.h"

using namespace testing;

namespace Bt {
namespace Core {

/*
class BothGMock : public I_Runnable , public I_SchedulingListener {
   public:
      MOCK_METHOD0(workcycle,Scheduling());
      MOCK_METHOD1(beforeStopModeSleep,void(SystemSleepConfiguration&));
      MOCK_METHOD1(afterStopModeSleep,void(SystemSleepWakeupReason));
};
*/

class WorkcycleTest : public ::testing::Test {
   public:
      WorkcycleTest() : mWorkcycle() {

      }

   protected:

      virtual void SetUp() {
      }

      virtual void TearDown() {

      }

      Workcycle mWorkcycle;

};


TEST_F(WorkcycleTest, oneWorkcycle) {

   RunnableGMock runnable1;
   RunnableGMock runnable2;
   RunnableGMock runnable3;

   EXPECT_CALL(runnable1,workcycle()).Times(3).WillRepeatedly(Return(Scheduling::immediately()));
   EXPECT_CALL(runnable2,workcycle()).Times(1).WillRepeatedly(Return(Scheduling::immediately()));
   EXPECT_CALL(runnable3,workcycle()).Times(2).WillRepeatedly(Return(Scheduling::immediately()));

   mWorkcycle.add(runnable1);

   mWorkcycle.oneWorkcycle();

   mWorkcycle.add(runnable2);
   mWorkcycle.add(runnable3);

   mWorkcycle.oneWorkcycle();

   mWorkcycle.remove(runnable2);

   mWorkcycle.oneWorkcycle();

   mWorkcycle.remove(runnable1);
   mWorkcycle.remove(runnable3);

}

/*
TEST_F(WorkcycleTest, registerBoth) {
   BothGMock bothMock;
   mWorkcycle.add(bothMock);

   EXPECT_CALL(bothMock,workcycle()).Times(1).WillRepeatedly(Return(Scheduling::inSeconds(5)));
   EXPECT_CALL(bothMock,beforeStopModeSleep(_)).Times(1);
   EXPECT_CALL(bothMock,afterStopModeSleep(_)).Times(1);



   mWorkcycle.oneWorkcycle();

}
*/


} // namespace Core
} // namespace Bt
