//*************************************************************************************************
//
//  BITTAILOR.CH
//
//*************************************************************************************************

#include <gtest/gtest.h>

#include "Bt/Core/Utility.h"

namespace Bt {
namespace Core {

TEST(UtilityTest, binaryAppend_int16_t) {
   int16_t input = 0x1213;
   const size_t BUFFER_SIZE = 100;
   uint8_t buffer[BUFFER_SIZE] = {0};
   binaryAppend(input,buffer);
   EXPECT_EQ((int)buffer[0], 0x12);
   EXPECT_EQ(buffer[1], 0x13);
   EXPECT_EQ(buffer[2], 0x0);
}

TEST(UtilityTest, binaryRead_int16_t) {
   const size_t BUFFER_SIZE = 100;
   uint8_t buffer[BUFFER_SIZE] = {0x12,0x13,0,0};
   int16_t output = binaryRead<int16_t>(buffer);
   EXPECT_EQ(output, 0x1213);
}

TEST(UtilityTest, binaryAppend_int8_t) {
   int8_t input = 0x34;
   const size_t BUFFER_SIZE = 100;
   uint8_t buffer[BUFFER_SIZE] = {0};
   binaryAppend(input,buffer);
   EXPECT_EQ((int)buffer[0], 0x34);
   EXPECT_EQ(buffer[1], 0x0);
}


} // namespace Core
} // namespace Bt
