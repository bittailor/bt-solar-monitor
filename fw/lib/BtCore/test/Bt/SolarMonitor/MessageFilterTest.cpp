//*************************************************************************************************
//
//  BITTAILOR.CH
//
//*************************************************************************************************

#include <functional>
#include <gtest/gtest.h>
#include <Bt/TestLogger.h>

#include "Bt/SolarMonitor/MessageFilter.h"

namespace Bt {
namespace SolarMonitor {

class MessageFilterTest : public ::testing::Test {

   protected:
      MessageFilterTest()
      :mMessageFilter(std::bind(&MessageFilterTest::comsume, this, std::placeholders::_1, std::placeholders::_2))
      ,mConsumed(false){

      }

      virtual void SetUp() {
      }

      virtual void TearDown() {

      }

      MessageFilter<6,3> mMessageFilter;
      bool mConsumed;

      void comsume(const MessageFilter<6,3>::MessageString* messages, size_t size) {
         for(int i = 0 ; i < size ; i++) {
            log() << i << ": " << messages[i] << std::endl;
         }
         mConsumed = true;
      }

};


TEST_F(MessageFilterTest, binaryAppend) {
   mMessageFilter.consume(std::array<float,6>{1.1,11.1,1.2,11.2,1.3,11.3});
   mMessageFilter.consume(std::array<float,6>{2.1,12.1,2.2,12.2,2.3,12.3});
   mMessageFilter.consume(std::array<float,6>{3.1,13.1,3.2,13.2,3.3,13.3});
   EXPECT_EQ(true,mConsumed);
}


} // namespace SolarMonitor
} // namespace Bt
