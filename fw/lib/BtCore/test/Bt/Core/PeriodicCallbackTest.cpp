//*************************************************************************************************
//
//  BITTAILOR.CH
//
//*************************************************************************************************

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Bt/Core/PeriodicCallback.h"
#include "Bt/Core/Singleton.h"
#include "Bt/Core/TimeGMock.h"

using namespace testing;

namespace Bt {
namespace Core {

class PeriodicCallbackTest : public ::testing::Test {

   protected:
      PeriodicCallbackTest()
      : mCallbackCounter(0)
      , mInstance(mTime)
      , mCallback(std::bind(&PeriodicCallbackTest::callback, this)) {
      }

      virtual void SetUp() {
      }

      virtual void TearDown() {
         delete mPeriodicCallback;
      }

      void callback() {
         mCallbackCounter++;
      }

      int mCallbackCounter;
      TimeGMock mTime;
      Singleton<I_Time>::Instance mInstance;
      std::function<void()> mCallback;
      PeriodicCallback* mPeriodicCallback;

};

TEST_F(PeriodicCallbackTest, SimplePeriodExact) {
   EXPECT_CALL(mTime,milliseconds())
      .WillOnce(Return(200))
      .WillOnce(Return(300));

   mPeriodicCallback = new PeriodicCallback(PeriodicCallback::MILLIS, 100, mCallback);
   Scheduling next = mPeriodicCallback->workcycle();

   EXPECT_EQ(1, mCallbackCounter);
   EXPECT_EQ(Scheduling::MILLIS_DELAY, next.type());
   EXPECT_EQ(100, next.delay());
}

TEST_F(PeriodicCallbackTest, SimplePeriodNotYet) {
   EXPECT_CALL(mTime,milliseconds())
      .WillOnce(Return(200))
      .WillOnce(Return(299));

   mPeriodicCallback = new PeriodicCallback(PeriodicCallback::MILLIS, 100, mCallback);
   Scheduling next = mPeriodicCallback->workcycle();

   EXPECT_EQ(0, mCallbackCounter);
   EXPECT_EQ(Scheduling::MILLIS_DELAY, next.type());
   EXPECT_EQ(1, next.delay());
}

TEST_F(PeriodicCallbackTest, SimplePeriodOverflowExact) {
   EXPECT_CALL(mTime,seconds())
      .WillOnce(Return(std::numeric_limits<uint32_t>::max() - 50))
      .WillOnce(Return(49));

   mPeriodicCallback = new PeriodicCallback(PeriodicCallback::SECONDS, 100, mCallback);
   Scheduling next = mPeriodicCallback->workcycle();

   EXPECT_EQ(Scheduling::SECONDS_DELAY, next.type());
   EXPECT_EQ(100, next.delay());
}

TEST_F(PeriodicCallbackTest, SimplePeriodOverflowNotYet) {
   EXPECT_CALL(mTime,seconds())
      .WillOnce(Return(std::numeric_limits<uint32_t>::max() - 50))
      .WillOnce(Return(40));

   mPeriodicCallback = new PeriodicCallback(PeriodicCallback::SECONDS, 100, mCallback);
   Scheduling next = mPeriodicCallback->workcycle();

   EXPECT_EQ(Scheduling::SECONDS_DELAY, next.type());
   EXPECT_EQ(9, next.delay());
}


} // namespace Core
} // namespace Bt
