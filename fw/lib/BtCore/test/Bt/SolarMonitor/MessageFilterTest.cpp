//*************************************************************************************************
//
//  BITTAILOR.CH
//
//*************************************************************************************************

#include <functional>

#include <gtest/gtest.h>

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
            std::cout << i << ": " << messages[i] << std::endl;
         }
         mConsumed = true;
      }

};


TEST_F(MessageFilterTest, binaryAppend) {
   mMessageFilter.consume(MeasurementRecord<6>{
      Reading{1.1,11.1},
      Reading{1.2,11.2},
      Reading{1.3,11.3},
      Reading{1.4,11.4},
      Reading{1.5,11.5},
      Reading{1.6,11.6}
   });
   mMessageFilter.consume(MeasurementRecord<6>{
      Reading{2.1,12.1},
      Reading{2.2,12.2},
      Reading{2.3,12.3},
      Reading{2.4,12.4},
      Reading{2.5,12.5},
      Reading{2.6,12.6}
   });
   mMessageFilter.consume(MeasurementRecord<6>{
      Reading{3.1,13.1},
      Reading{3.2,13.2},
      Reading{3.3,13.3},
      Reading{3.4,13.4},
      Reading{3.5,13.5},
      Reading{3.6,13.6}
   });

   EXPECT_EQ(true,mConsumed);
}


} // namespace SolarMonitor
} // namespace Bt
