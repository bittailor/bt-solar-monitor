//*************************************************************************************************
//
//  BITTAILOR.CH
//
//*************************************************************************************************

#include <gtest/gtest.h>

#include "Bt/Core/CircularBuffer.h"

namespace Bt {
namespace Core {
namespace {

   void FillLessThanSize(CircularBuffer<int,5>& pBuffer) {
      pBuffer.push(1);
      pBuffer.push(2);
      pBuffer.push(3);
   }

   void FillMoreThanSize(CircularBuffer<int,5>& pBuffer) {
      pBuffer.push(1);
      pBuffer.push(2);
      pBuffer.push(3);
      pBuffer.push(4);
      pBuffer.push(5);
      pBuffer.push(6);
      pBuffer.push(7);
   }

} // namespace


TEST(CircularBufferTest, emptyOnEmpty) {
   CircularBuffer<int,5> buffer;
   EXPECT_TRUE(buffer.empty());
}

TEST(CircularBufferTest, countOnEmpty) {
   CircularBuffer<int,5> buffer;
   EXPECT_EQ(0, buffer.count());
}

TEST(CircularBufferTest, peakOnEmpty) {
   CircularBuffer<int,5> buffer;
   EXPECT_EQ(nullptr, buffer.peak());
}

TEST(CircularBufferTest, popOnEmpty) {
   CircularBuffer<int,5> buffer;
   buffer.pop();
}

TEST(CircularBufferTest, emptyOnFillLessThanSize) {
   CircularBuffer<int,5> buffer;
   FillLessThanSize(buffer);
   EXPECT_FALSE(buffer.empty());
}

TEST(CircularBufferTest, countOnFillLessThanSize) {
   CircularBuffer<int,5> buffer;
   FillLessThanSize(buffer);
   EXPECT_EQ(3, buffer.count());
}

TEST(CircularBufferTest, peakOnFillLessThanSize) {
   CircularBuffer<int,5> buffer;
   FillLessThanSize(buffer);
   ASSERT_NE(nullptr, buffer.peak());
   EXPECT_EQ(1, *buffer.peak());
}

TEST(CircularBufferTest, popOnFillLessThanSize) {
   CircularBuffer<int,5> buffer;
   FillLessThanSize(buffer);
   buffer.pop();
   EXPECT_EQ(2, *buffer.peak());
   EXPECT_EQ(2, buffer.count());

}

TEST(CircularBufferTest, popAllOnFillLessThanSize) {
   CircularBuffer<int,5> buffer;
   FillLessThanSize(buffer);
   EXPECT_EQ(1, *buffer.peak());
   buffer.pop();
   EXPECT_EQ(2, *buffer.peak());
   buffer.pop();
   EXPECT_EQ(3, *buffer.peak());
   buffer.pop();
   EXPECT_TRUE(buffer.empty());
}

TEST(CircularBufferTest, emptyOnFillMoreThanSize) {
   CircularBuffer<int,5> buffer;
   FillMoreThanSize(buffer);
   EXPECT_FALSE(buffer.empty());
}

TEST(CircularBufferTest, countOnFillMoreThanSize) {
   CircularBuffer<int,5> buffer;
   FillMoreThanSize(buffer);
   EXPECT_EQ(5, buffer.count());
}

TEST(CircularBufferTest, peakOnFillMoreThanSize) {
   CircularBuffer<int,5> buffer;
   FillMoreThanSize(buffer);
   ASSERT_NE(nullptr, buffer.peak());
   EXPECT_EQ(3, *buffer.peak());
}

TEST(CircularBufferTest, popOnFillMoreThanSize) {
   CircularBuffer<int,5> buffer;
   FillMoreThanSize(buffer);
   buffer.pop();
   EXPECT_EQ(4, *buffer.peak());
   EXPECT_EQ(4, buffer.count());

}

TEST(CircularBufferTest, popAllOnMoreLessThanSize) {
   CircularBuffer<int,5> buffer;
   FillMoreThanSize(buffer);
   EXPECT_EQ(3, *buffer.peak());
   buffer.pop();
   EXPECT_EQ(4, *buffer.peak());
   buffer.pop();
   EXPECT_EQ(5, *buffer.peak());
   buffer.pop();
   EXPECT_EQ(6, *buffer.peak());
   buffer.pop();
   EXPECT_EQ(7, *buffer.peak());
   buffer.pop();
   EXPECT_TRUE(buffer.empty());
}

TEST(CircularBufferTest, pushAndPopSzenario) {
   CircularBuffer<int,5> buffer;

   buffer.push(1);
   buffer.push(2);
   buffer.push(3);

   EXPECT_EQ(1, *buffer.peak());
   buffer.pop();

   buffer.push(4);
   buffer.push(5);
   buffer.push(6);

   EXPECT_EQ(2, *buffer.peak());
   buffer.push(7);
   EXPECT_EQ(3, *buffer.peak());
   buffer.pop();
   EXPECT_EQ(4, *buffer.peak());
   buffer.push(8);
   EXPECT_EQ(4, *buffer.peak());

   buffer.push(9);
   buffer.push(10);

   EXPECT_EQ(6, *buffer.peak());
   buffer.pop();
   EXPECT_EQ(7, *buffer.peak());
   buffer.pop();
   buffer.pop();
   buffer.pop();
   buffer.pop();
   buffer.pop();

   EXPECT_TRUE(buffer.empty());

   buffer.push(11);
   buffer.push(12);
   buffer.push(13);

   EXPECT_EQ(11, *buffer.peak());
   buffer.pop();
   EXPECT_EQ(12, *buffer.peak());
   buffer.pop();

   EXPECT_FALSE(buffer.empty());
   EXPECT_EQ(1, buffer.count());
}



} // namespace Core
} // namespace Bt
