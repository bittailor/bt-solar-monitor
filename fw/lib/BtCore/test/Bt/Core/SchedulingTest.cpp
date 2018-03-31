//*************************************************************************************************
//
//  BITTAILOR.CH
//
//*************************************************************************************************

#include <gtest/gtest.h>

#include "Bt/Core/Scheduling.h"

namespace Bt {
namespace Core {



TEST(SchedulingTest, orderimmediately) {
   EXPECT_TRUE(Scheduling::immediately() < Scheduling::inMillis(0));
   EXPECT_TRUE(Scheduling::immediately() < Scheduling::inMillis(1));
   EXPECT_TRUE(Scheduling::immediately() < Scheduling::inMillis(9999));
   EXPECT_TRUE(Scheduling::immediately() < Scheduling::inSeconds(0));
   EXPECT_TRUE(Scheduling::immediately() < Scheduling::inSeconds(1));
   EXPECT_TRUE(Scheduling::immediately() < Scheduling::inSeconds(9999));
   EXPECT_TRUE(Scheduling::immediately() < Scheduling::never());
}

TEST(SchedulingTest, orderinMillis) {

   EXPECT_FALSE(Scheduling::inMillis(0) < Scheduling::immediately());
   EXPECT_FALSE(Scheduling::inMillis(1) < Scheduling::immediately());
   EXPECT_FALSE(Scheduling::inMillis(9999) < Scheduling::immediately());

   EXPECT_FALSE(Scheduling::inMillis(9999) < Scheduling::inMillis(1));

   EXPECT_TRUE(Scheduling::inMillis(0) < Scheduling::inMillis(1));
   EXPECT_TRUE(Scheduling::inMillis(1000) < Scheduling::inMillis(1001));

   EXPECT_TRUE(Scheduling::inMillis(0) < Scheduling::inSeconds(0));
   EXPECT_TRUE(Scheduling::inMillis(999) < Scheduling::inSeconds(1));
   EXPECT_TRUE(Scheduling::inMillis(0) < Scheduling::never());

}


} // namespace Core
} // namespace Bt
