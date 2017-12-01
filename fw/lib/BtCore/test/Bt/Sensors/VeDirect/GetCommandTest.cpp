//*************************************************************************************************
//
//  BITTAILOR.CH
//
//*************************************************************************************************

#include <gtest/gtest.h>

#include "Bt/Sensors/VeDirect/GetCommand.h"
#include "Bt/Sensors/VeDirect/Register.h"

namespace Bt {
namespace Sensors {
namespace VeDirect {

using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrEq;

TEST(GetCommandTest, getBatteryMaximumCurrent) {
   StreamMock stream;
   {
      InSequence seq;
      EXPECT_CALL(stream, available())
         .WillRepeatedly(Return(false));
      EXPECT_CALL(stream, read())
         .WillRepeatedly(Return(-1));
      EXPECT_CALL(stream, println(StrEq(":7F0ED0071")));
      EXPECT_CALL(stream, available())
         .WillRepeatedly(Return(true));
      // :7F0ED009600DB
      EXPECT_CALL(stream, read())
         .WillOnce(Return(':'))
         .WillOnce(Return('7'))
         .WillOnce(Return('F'))
         .WillOnce(Return('0'))
         .WillOnce(Return('E'))
         .WillOnce(Return('D'))
         .WillOnce(Return('0'))
         .WillOnce(Return('0'))
         .WillOnce(Return('9'))
         .WillOnce(Return('6'))
         .WillOnce(Return('0'))
         .WillOnce(Return('0'))
         .WillOnce(Return('D'))
         .WillOnce(Return('B'))
         .WillOnce(Return('\n'));
   }

   GetCommand getCommand{Register::BatteryMaximumCurrent};
   std::pair<bool,int16_t> result = getCommand.execute(stream);

   EXPECT_TRUE(result.first);
   EXPECT_EQ(150,result.second);
}

TEST(GetCommandTest, getBatteryMaximumCurrentWithDelays) {
   StreamMock stream;
   {
      InSequence seq;
      EXPECT_CALL(stream, available())
         .WillRepeatedly(Return(false));
      EXPECT_CALL(stream, read())
         .WillRepeatedly(Return(-1));
      EXPECT_CALL(stream, println(StrEq(":7F0ED0071")));
      EXPECT_CALL(stream, available())
         .WillRepeatedly(Return(true));
      // :7F0ED009600DB
      EXPECT_CALL(stream, read())
         .WillOnce(Return(-1))
         .WillOnce(Return(-1))
         .WillOnce(Return(-1))
         .WillOnce(Return(':'))
         .WillOnce(Return('7'))
         .WillOnce(Return('F'))
         .WillOnce(Return('0'))
         .WillOnce(Return(-1))
         .WillOnce(Return('E'))
         .WillOnce(Return('D'))
         .WillOnce(Return('0'))
         .WillOnce(Return('0'))
         .WillOnce(Return(-1))
         .WillOnce(Return(-1))
         .WillOnce(Return('9'))
         .WillOnce(Return('6'))
         .WillOnce(Return('0'))
         .WillOnce(Return(-1))
         .WillOnce(Return(-1))
         .WillOnce(Return('0'))
         .WillOnce(Return('D'))
         .WillOnce(Return('B'))
         .WillOnce(Return(-1))
         .WillOnce(Return(-1))
         .WillOnce(Return('\n'));
   }

   GetCommand getCommand{Register::BatteryMaximumCurrent};
   std::pair<bool,int16_t> result = getCommand.execute(stream);

   EXPECT_TRUE(result.first);
   EXPECT_EQ(150,result.second);
}

TEST(GetCommandTest, getBatteryMaximumCurrentWithAsyncNoise) {
   StreamMock stream;
   {
      InSequence seq;
      EXPECT_CALL(stream, available())
         .WillRepeatedly(Return(false));
      EXPECT_CALL(stream, read())
         .WillRepeatedly(Return(-1));
      EXPECT_CALL(stream, println(StrEq(":7F0ED0071")));
      EXPECT_CALL(stream, available())
         .WillRepeatedly(Return(true));
      // :7F0ED009600DB
      EXPECT_CALL(stream, read())
         .WillOnce(Return(':'))
         .WillOnce(Return('A'))
         .WillOnce(Return('0'))
         .WillOnce(Return('1'))
         .WillOnce(Return('0'))
         .WillOnce(Return('2'))
         .WillOnce(Return('0'))
         .WillOnce(Return('0'))
         .WillOnce(Return('0'))
         .WillOnce(Return('5'))
         .WillOnce(Return('4'))
         .WillOnce(Return('3'))
         .WillOnce(Return('\n'))

         .WillOnce(Return(':'))
         .WillOnce(Return('7'))
         .WillOnce(Return('F'))
         .WillOnce(Return('0'))
         .WillOnce(Return('E'))
         .WillOnce(Return('D'))
         .WillOnce(Return('0'))
         .WillOnce(Return('0'))
         .WillOnce(Return('9'))
         .WillOnce(Return('6'))
         .WillOnce(Return('0'))
         .WillOnce(Return('0'))
         .WillOnce(Return('D'))
         .WillOnce(Return('B'))
         .WillOnce(Return('\n'));
   }

   GetCommand getCommand{Register::BatteryMaximumCurrent};
   std::pair<bool,int16_t> result = getCommand.execute(stream);

   EXPECT_TRUE(result.first);
   EXPECT_EQ(150,result.second);
}

TEST(GetCommandTest, getBatteryMaximumCurrentWithNoiseToFlush) {
   StreamMock stream;
   {
      InSequence seq;
      EXPECT_CALL(stream, available()).WillOnce(Return(true));
      EXPECT_CALL(stream, read()).WillOnce(Return('x'));
      EXPECT_CALL(stream, available()).WillOnce(Return(true));
      EXPECT_CALL(stream, read()).WillOnce(Return('y'));
      EXPECT_CALL(stream, available()).WillOnce(Return(true));
      EXPECT_CALL(stream, read()).WillOnce(Return('z'));
      EXPECT_CALL(stream, available())
         .WillRepeatedly(Return(false));
      EXPECT_CALL(stream, read())
         .WillRepeatedly(Return(-1));
      EXPECT_CALL(stream, println(StrEq(":7F0ED0071")));
      EXPECT_CALL(stream, available())
         .WillRepeatedly(Return(true));
      // :7F0ED009600DB
      EXPECT_CALL(stream, read())
         .WillOnce(Return(':'))
         .WillOnce(Return('7'))
         .WillOnce(Return('F'))
         .WillOnce(Return('0'))
         .WillOnce(Return('E'))
         .WillOnce(Return('D'))
         .WillOnce(Return('0'))
         .WillOnce(Return('0'))
         .WillOnce(Return('9'))
         .WillOnce(Return('6'))
         .WillOnce(Return('0'))
         .WillOnce(Return('0'))
         .WillOnce(Return('D'))
         .WillOnce(Return('B'))
         .WillOnce(Return('\n'));
   }

   GetCommand getCommand{Register::BatteryMaximumCurrent};
   std::pair<bool,int16_t> result = getCommand.execute(stream);

   EXPECT_TRUE(result.first);
   EXPECT_EQ(150,result.second);
}

TEST(GetCommandTest, getBatteryMaximumCurrentWithWrongCheckSum) {
   StreamMock stream;
   {
      InSequence seq;
      EXPECT_CALL(stream, available())
         .WillRepeatedly(Return(false));
      EXPECT_CALL(stream, read())
         .WillRepeatedly(Return(-1));
      EXPECT_CALL(stream, println(StrEq(":7F0ED0071")));
      EXPECT_CALL(stream, available())
         .WillRepeatedly(Return(true));
      // :7F0ED009600DB
      EXPECT_CALL(stream, read())
         .WillOnce(Return(':'))
         .WillOnce(Return('7'))
         .WillOnce(Return('F'))
         .WillOnce(Return('0'))
         .WillOnce(Return('E'))
         .WillOnce(Return('D'))
         .WillOnce(Return('0'))
         .WillOnce(Return('0'))
         .WillOnce(Return('9'))
         .WillOnce(Return('6'))
         .WillOnce(Return('0'))
         .WillOnce(Return('0'))
         .WillOnce(Return('B'))
         .WillOnce(Return('B'))
         .WillOnce(Return('\n'));
   }

   GetCommand getCommand{Register::BatteryMaximumCurrent};
   std::pair<bool,int16_t> result = getCommand.execute(stream);

   EXPECT_FALSE(result.first);
}

TEST(GetCommandTest, getBatteryMaximumCurrentWithNoEnd) {
   StreamMock stream;
   {
      InSequence seq;
      EXPECT_CALL(stream, available())
         .WillRepeatedly(Return(false));
      EXPECT_CALL(stream, read())
         .WillRepeatedly(Return(-1));
      EXPECT_CALL(stream, println(StrEq(":7F0ED0071")));
      EXPECT_CALL(stream, available())
         .WillRepeatedly(Return(true));
      // :7F0ED009600DB
      EXPECT_CALL(stream, read())
         .WillOnce(Return(':'))
         .WillRepeatedly(Return('7'));
   }

   GetCommand getCommand{Register::BatteryMaximumCurrent};
   std::pair<bool,int16_t> result = getCommand.execute(stream);

   EXPECT_FALSE(result.first);
}

} // namespace VeDirect
} // namespace Sensors
} // namespace Bt
