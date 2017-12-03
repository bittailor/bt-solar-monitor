//*************************************************************************************************
//
//  BITTAILOR.CH
//
//*************************************************************************************************

#include <gtest/gtest.h>

#include "Bt/Sensors/VeDirect/GetCommand.h"
#include "Bt/Sensors/VeDirect/Register.h"
#include "Bt/Sensors/VeDirect/Registers.h"

namespace Bt {
namespace Sensors {
namespace VeDirect {

using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrEq;

void setupCommandResponse(StreamMock& pStreamMock, const char* pCommand, const char* pResponse) {
   {
      InSequence seq;
      EXPECT_CALL(pStreamMock, available())
         .WillRepeatedly(Return(false));
      EXPECT_CALL(pStreamMock, read())
         .WillRepeatedly(Return(-1));
      EXPECT_CALL(pStreamMock, println(StrEq(pCommand)));
      EXPECT_CALL(pStreamMock, available())
         .WillRepeatedly(Return(true));
      for(const char* it = pResponse; *it; ++it) {
         EXPECT_CALL(pStreamMock, read()).WillOnce(Return(*it));
      };
      EXPECT_CALL(pStreamMock, read()).WillOnce(Return('\n'));
   }
}

TEST(GetCommandTest, getBatteryMaximumCurrent_150) {
   StreamMock stream;
   setupCommandResponse(stream, ":7F0ED0071", ":7F0ED009600DB");

   Core::Result<uint16_t> result = Registers::batteryMaximumCurrent().get(stream);

   EXPECT_TRUE(result);
   EXPECT_EQ(150, result.value());
}

TEST(GetCommandTest, getBatteryMaximumCurrent_30259) {
   StreamMock stream;
   setupCommandResponse(stream, ":7F0ED0071", ":7F0ED003376C8");

   Core::Result<uint16_t> result = Registers::batteryMaximumCurrent().get(stream);

   EXPECT_TRUE(result);
   EXPECT_EQ(30259, result.value());
}

TEST(GetCommandTest, getPanelPower_0) {
   StreamMock stream;
   setupCommandResponse(stream, ":7BCED00A5", ":7BCED0000000000A5");

   Core::Result<uint32_t> result = Registers::panelPower().get(stream);

   EXPECT_TRUE(result);
   EXPECT_EQ(0, result.value());
}

TEST(GetCommandTest, getPanelPower_4294967295) {
   StreamMock stream;
   setupCommandResponse(stream, ":7BCED00A5", ":7BCED00FFFFFFFFA9");

   Core::Result<uint32_t> result = Registers::panelPower().get(stream);

   EXPECT_TRUE(result);
   EXPECT_EQ(4294967295, result.value());
}

TEST(GetCommandTest, getPanelPower_1732584193) {
   StreamMock stream;
   setupCommandResponse(stream, ":7BCED00A5", ":7BCED0001234567D5");

   Core::Result<uint32_t> result = Registers::panelPower().get(stream);

   EXPECT_TRUE(result);
   EXPECT_EQ(1732584193, result.value());
}

TEST(GetCommandTest, getPanelPower_4275878552) {
   StreamMock stream;
   setupCommandResponse(stream, ":7BCED00A5", ":7BCED0098BADCFE79");

   Core::Result<uint32_t> result = Registers::panelPower().get(stream);

   EXPECT_TRUE(result);
   EXPECT_EQ(4275878552, result.value());
}

TEST(GetCommandTest, getLoadOutputState_True) {
   StreamMock stream;
   setupCommandResponse(stream, ":7A8ED00B9", ":7A8ED0001B8");

   Core::Result<bool> result = Registers::loadOutputState().get(stream);

   EXPECT_TRUE(result);
   EXPECT_EQ(true, result.value());
}

TEST(GetCommandTest, getLoadOutputState_False) {
   StreamMock stream;
   setupCommandResponse(stream, ":7A8ED00B9", ":7A8ED0000B9");

   Core::Result<bool> result = Registers::loadOutputState().get(stream);

   EXPECT_TRUE(result);
   EXPECT_EQ(false, result.value());
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

   Core::Result<uint16_t> result = Registers::batteryMaximumCurrent().get(stream);

   EXPECT_TRUE(result);
   EXPECT_EQ(150, result.value());
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

   Core::Result<uint16_t> result = Registers::batteryMaximumCurrent().get(stream);

   EXPECT_TRUE(result);
   EXPECT_EQ(150, result.value());
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

   Core::Result<uint16_t> result = Registers::batteryMaximumCurrent().get(stream);

   EXPECT_TRUE(result);
   EXPECT_EQ(150, result.value());
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

   Core::Result<uint16_t> result = Registers::batteryMaximumCurrent().get(stream);

   EXPECT_FALSE(result);
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

   Core::Result<uint16_t> result = Registers::batteryMaximumCurrent().get(stream);

   EXPECT_FALSE(result);
}

} // namespace VeDirect
} // namespace Sensors
} // namespace Bt
