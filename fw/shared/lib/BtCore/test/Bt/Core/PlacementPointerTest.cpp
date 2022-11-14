//*************************************************************************************************
//
//  BITTAILOR.CH
//
//*************************************************************************************************

#include <gtest/gtest.h>

#include "Bt/Core/Utility.h"
#include "Bt/Core/PlacementPointer.h"

namespace Bt {
namespace Core {
namespace {

   class StubClassEmpty {
      public:
         int answer(){return 42;}
   };

   class StubClassInt {
      public:
         StubClassInt(int pInteger):mInteger(pInteger){}

         int integer(){return mInteger;}
      
      private:
         int mInteger;
   };


}

TEST(PlacementPointerTest, createAndDispose_EmptyStub) {
   PlacementPointer<StubClassEmpty> empty;
   EXPECT_FALSE(empty);
   empty.create();
   EXPECT_TRUE(empty);
   empty.dispose();
   EXPECT_FALSE(empty);
}

TEST(PlacementPointerTest, call_EmptyStub) {
   PlacementPointer<StubClassEmpty> cut;
   cut.create();
   EXPECT_EQ(42, cut->answer());
}

TEST(PlacementPointerTest, createAndDispose_IntStub) {
   PlacementPointer<StubClassInt> cut;
   EXPECT_FALSE(cut);
   cut.create(4711);
   EXPECT_TRUE(cut);
   cut.dispose();
   EXPECT_FALSE(cut);
}

TEST(PlacementPointerTest, call_IntStub) {
   PlacementPointer<StubClassInt> cut;
   cut.create(4711);
   EXPECT_EQ(4711, cut->integer());
}




} // namespace Core
} // namespace Bt
