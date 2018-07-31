//*************************************************************************************************
//
//  BITTAILOR.CH
//
//*************************************************************************************************

#include <gtest/gtest.h>

#include "Bt/SolarMonitor/MessageBufferSink.h"

namespace Bt {
namespace SolarMonitor {

TEST(MessageBufferSinkTest, consumeAndPopOne) {
   MessageBufferSink<5> buffer;

   buffer.consume("1");

   EXPECT_STREQ("1",buffer.peak());
   buffer.pop();
   EXPECT_TRUE(buffer.empty());
}

TEST(MessageBufferSinkTest, consumeAndPopThree) {
   MessageBufferSink<5> buffer;

   buffer.consume("1");
   buffer.consume("2");
   buffer.consume("3");

   EXPECT_STREQ("1",buffer.peak());
   buffer.pop();
   EXPECT_STREQ("2",buffer.peak());
   buffer.pop();
   EXPECT_STREQ("3",buffer.peak());
   buffer.pop();
   EXPECT_TRUE(buffer.empty());
}

TEST(MessageBufferSinkTest, consumeAndPopMoreThanSpace) {
   MessageBufferSink<5> buffer;

   buffer.consume("1");
   buffer.consume("2");
   buffer.consume("3");
   buffer.consume("4");
   buffer.consume("5");
   buffer.consume("6");


   EXPECT_STREQ("2",buffer.peak());
   buffer.pop();
   EXPECT_STREQ("3",buffer.peak());
   buffer.pop();
   EXPECT_STREQ("4",buffer.peak());
   buffer.pop();
   EXPECT_STREQ("5",buffer.peak());
   buffer.pop();
   EXPECT_STREQ("6",buffer.peak());
   buffer.pop();
   EXPECT_TRUE(buffer.empty());
}

TEST(MessageBufferSinkTest, peakEmpty) {
   MessageBufferSink<5> buffer;

   EXPECT_STREQ(nullptr,buffer.peak());
   buffer.pop();
   EXPECT_TRUE(buffer.empty());
}


} // namespace SolarMonitor
} // namespace Bt
