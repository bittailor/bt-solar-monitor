//*************************************************************************************************
//
//  BITTAILOR.CH
//
//*************************************************************************************************

#include <gtest/gtest.h>

#include "Bt/Sensors/VeDirect/Utility.h"

namespace Bt {
namespace Sensors {
namespace VeDirect {

TEST(UtilityTests, hexToByte) {
   EXPECT_EQ(0x55, hexToByte("55"));
   EXPECT_EQ(0x00, hexToByte("00"));
   EXPECT_EQ(0xA0, hexToByte("A0"));
   EXPECT_EQ(0x0A, hexToByte("0A"));
   EXPECT_EQ(0xFF, hexToByte("FF"));
   EXPECT_EQ(0x10, hexToByte("10"));
   EXPECT_EQ(0x07, hexToByte(":7"));
}

TEST(UtilityTests, byteToHex) {
   char buffer [3] = {0};
   ASSERT_STREQ("55", byteToHex(0x55,buffer));
   ASSERT_STREQ("00", byteToHex(0x00,buffer));
   ASSERT_STREQ("A0", byteToHex(0xA0,buffer));
   ASSERT_STREQ("0A", byteToHex(0x0A,buffer));
   ASSERT_STREQ("FF", byteToHex(0xFF,buffer));
   ASSERT_STREQ("10", byteToHex(0x10,buffer));
}

TEST(UtilityTests, checksum) {
   EXPECT_EQ(0x8C, checksum(":7EDD500"));
   EXPECT_EQ(0xA6, checksum(":7BBED00"));
   EXPECT_EQ(0x8A, checksum(":7D7ED00"));
   EXPECT_EQ(0x54, checksum(":1"));
}

TEST(UtilityTests, strAppendLittleEndianHex_int16_t) {
   char buffer [20] = {0};
   int16_t value = 100;
   ASSERT_STREQ("6400", strAppendLittleEndianHex(buffer, value));
}

TEST(UtilityTests, strAppendLittleEndianHex_uint32_t) {
   char buffer [20] = {0};
   uint32_t value = 3740974200;
   ASSERT_STREQ("78BCFADE", strAppendLittleEndianHex(buffer, value));
}

TEST(UtilityTests, strAppendLittleEndianHex_uint32_t_append) {
   char buffer [20] = {'h', 'e', 'l', 'l', 'o', 0};
   uint32_t value = 3740974200;
   ASSERT_STREQ("hello78BCFADE", strAppendLittleEndianHex(buffer, value));
}

TEST(UtilityTests, strAppendLittleEndianHex_int8_t_1) {
   char buffer [20] = {0};
   int8_t value = 1;
   ASSERT_STREQ("01", strAppendLittleEndianHex(buffer, value));
}

TEST(UtilityTests, strAppendLittleEndianHex_int8_t_0) {
   char buffer [20] = {0};
   int8_t value = 0;
   ASSERT_STREQ("00", strAppendLittleEndianHex(buffer, value));
}

} // namespace VeDirect
} // namespace Sensors
} // namespace Bt
