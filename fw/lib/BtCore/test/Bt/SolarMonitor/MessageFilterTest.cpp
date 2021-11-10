//*************************************************************************************************
//
//  BITTAILOR.CH
//
//*************************************************************************************************

#include <functional>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <Bt/TestLogger.h>

#include "Bt/SolarMonitor/MessageFilter.h"

using ::testing::_;
using ::testing::Exactly;
using ::testing::Truly;

namespace Bt {
namespace SolarMonitor {
namespace {

   class ConsumerMock {
      public:
         MOCK_METHOD1(consume, void(const char*));
   };

   template<typename Out>
   void split(const std::string &s, char delim, Out result) {
       std::stringstream ss(s);
       std::string item;
       while (std::getline(ss, item, delim)) {
           *(result++) = item;
       }
   }

   std::vector<std::string> split(const std::string &s, char delim) {
       std::vector<std::string> elems;
       split(s, delim, std::back_inserter(elems));
       return elems;
   }


}

TEST(MessageFilterTest, checkForwardOnFull) {

   ConsumerMock consumer;
   MessageFilter<6,3> messageFilter(std::bind(&ConsumerMock::consume, &consumer, std::placeholders::_1));

   EXPECT_CALL(consumer, consume(_)).Times(Exactly(1));

   messageFilter.consume(std::array<float,6>{1.1,11.1,1.2,11.2,1.3,11.3});
   messageFilter.consume(std::array<float,6>{2.1,12.1,2.2,12.2,2.3,12.3});
   messageFilter.consume(std::array<float,6>{3.1,13.1,3.2,13.2,3.3,13.3});
}

TEST(MessageFilterTest, checkConstants) {

   ConsumerMock consumer;
   MessageFilter<4,12> messageFilter(std::bind(&ConsumerMock::consume, &consumer, std::placeholders::_1));

   EXPECT_CALL(consumer, consume(_)).Times(Exactly(1));

   messageFilter.consume(std::array<float,4>{1});
   messageFilter.consume(std::array<float,4>{2});
   messageFilter.consume(std::array<float,4>{3});
   messageFilter.consume(std::array<float,4>{4});
   messageFilter.consume(std::array<float,4>{5});
   messageFilter.consume(std::array<float,4>{6});
   messageFilter.consume(std::array<float,4>{7});
   messageFilter.consume(std::array<float,4>{8});
   messageFilter.consume(std::array<float,4>{9});
   messageFilter.consume(std::array<float,4>{10});
   messageFilter.consume(std::array<float,4>{11});
   messageFilter.consume(std::array<float,4>{12});


}

TEST(MessageFilterTest, checkDataTwoFloats) {

   ConsumerMock consumer;
   MessageFilter<2,1> messageFilter(std::bind(&ConsumerMock::consume, &consumer, std::placeholders::_1));

   const char* messages[]={"1"};

   EXPECT_CALL(consumer, consume(Truly([](const char* message) -> bool {
      std::vector<std::string> parts = split(std::string(message),'|');
      EXPECT_EQ(4, parts.size());
      EXPECT_STREQ("1wx8*", parts[2].c_str());
      return true;
   })))
   .Times(Exactly(1));

   messageFilter.consume(std::array<float,2>{1.1,2.2});
}


} // namespace SolarMonitor
} // namespace Bt
