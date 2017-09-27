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

class Z85Test : public ::testing::Test {

   protected:

      virtual void SetUp() {
      }

      virtual void TearDown() {

      }

      Z85 mZ85;

};


TEST_F(Z85Test, encode) {
   EXPECT_EQ(1,0);
}


} // namespace Z85
} // namespace Encoding
} // namespace Bt
