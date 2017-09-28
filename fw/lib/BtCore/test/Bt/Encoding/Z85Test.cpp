//*************************************************************************************************
//
//  BITTAILOR.CH
//
//*************************************************************************************************

#include <gtest/gtest.h>

#include "Bt/Encoding/Z85.h"

namespace Bt {
namespace Encoding {
namespace Z85 {


TEST(Z85Test, encode) {
   const size_t BUFFER_SIZE = 100;
   char buffer[BUFFER_SIZE];
   uint8_t input[] = {0x86 , 0x4F , 0xD2 , 0x6F , 0xB5 , 0x59 , 0xF7 , 0x5B};
   encode(input, sizeof(input)/sizeof(input[0]), buffer, BUFFER_SIZE);
   EXPECT_STREQ(buffer,"HelloWorld");
}


} // namespace Z85
} // namespace Encoding
} // namespace Bt
