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
         MOCK_METHOD2(consume, void(const char *const *, size_t));
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
   MessageFilter<6,3> messageFilter(std::bind(&ConsumerMock::consume, &consumer, std::placeholders::_1, std::placeholders::_2));

   EXPECT_CALL(consumer, consume(_,_)).Times(Exactly(1));

   messageFilter.consume(std::array<float,6>{1.1,11.1,1.2,11.2,1.3,11.3});
   messageFilter.consume(std::array<float,6>{2.1,12.1,2.2,12.2,2.3,12.3});
   messageFilter.consume(std::array<float,6>{3.1,13.1,3.2,13.2,3.3,13.3});
}

TEST(MessageFilterTest, checkConstants) {

   ConsumerMock consumer;
   MessageFilter<10,12> messageFilter(std::bind(&ConsumerMock::consume, &consumer, std::placeholders::_1, std::placeholders::_2));

   EXPECT_CALL(consumer, consume(_,_)).Times(Exactly(1));

   messageFilter.consume(std::array<float,10>{1});
   messageFilter.consume(std::array<float,10>{2});
   messageFilter.consume(std::array<float,10>{3});
   messageFilter.consume(std::array<float,10>{4});
   messageFilter.consume(std::array<float,10>{5});
   messageFilter.consume(std::array<float,10>{6});
   messageFilter.consume(std::array<float,10>{7});
   messageFilter.consume(std::array<float,10>{8});
   messageFilter.consume(std::array<float,10>{9});
   messageFilter.consume(std::array<float,10>{10});
   messageFilter.consume(std::array<float,10>{11});
   messageFilter.consume(std::array<float,10>{12});


}

TEST(MessageFilterTest, checkDataTwoFloats) {

   ConsumerMock consumer;
   MessageFilter<2,1> messageFilter(std::bind(&ConsumerMock::consume, &consumer, std::placeholders::_1, std::placeholders::_2));

   const char* messages[]={"1"};

   EXPECT_CALL(consumer, consume(Truly([](const char *const * messages) -> bool {
      std::vector<std::string> parts = split(std::string(messages[0]),'|');
      EXPECT_EQ(6, parts.size());
      EXPECT_STREQ("1wx8*", parts[4].c_str());
      return true;
   }),1))
   .Times(Exactly(1));

   messageFilter.consume(std::array<float,2>{1.1,2.2});
}


} // namespace SolarMonitor
} // namespace Bt
