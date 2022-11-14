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

class WorkcycleTest : public ::testing::Test {
   public:
      WorkcycleTest() : mWorkcycle(10) {

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



} // namespace Core
} // namespace Bt
