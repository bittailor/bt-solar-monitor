//*************************************************************************************************
//
//  BITTAILOR.CH
//
//*************************************************************************************************

#include <array>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <Bt/TestLogger.h>


#include "Bt/Core/Utility.h"
#include "Bt/Encoding/Z85.h"


namespace Bt {
namespace Encoding {
namespace Z85 {


using Bt::Core::sizeOfArry;
using testing::ElementsAreArray;

TEST(Z85Test, encode) {
   const size_t BUFFER_SIZE = 100;
   char buffer[BUFFER_SIZE];
   uint8_t input[] = {0x86 , 0x4F , 0xD2 , 0x6F , 0xB5 , 0x59 , 0xF7 , 0x5B};
   encode(input, sizeOfArry(input), buffer, BUFFER_SIZE);
   EXPECT_STREQ(buffer,"HelloWorld");
}

TEST(Z85Test, encode_int16_t_1) {
   const size_t BUFFER_SIZE = 100;
   char buffer[BUFFER_SIZE];
   uint8_t input[2*sizeof(int16_t)];
   Bt::Core::binaryAppend(static_cast<uint16_t>(1),input);
   Bt::Core::binaryAppend(static_cast<uint16_t>(2),input+sizeof(uint16_t));
   encode(input, sizeOfArry(input), buffer, BUFFER_SIZE);
   EXPECT_STREQ(buffer,"00963");
}

TEST(Z85Test, encode_int16_t_2) {
   const size_t BUFFER_SIZE = 100;
   char buffer[BUFFER_SIZE];
   uint8_t input[2*sizeof(int16_t)];
   Bt::Core::binaryAppend(static_cast<uint16_t>(1100),input);
   Bt::Core::binaryAppend(static_cast<uint16_t>(2200),input+sizeof(uint16_t));
   encode(input, sizeOfArry(input), buffer, BUFFER_SIZE);
   EXPECT_STREQ(buffer,"1wx8*");
}

TEST(Z85Test, decode) {
   const size_t BUFFER_SIZE = 100;
   uint8_t buffer[BUFFER_SIZE];
   size_t lenght = decode("HelloWorld", buffer, BUFFER_SIZE);
   EXPECT_THAT(std::vector<uint8_t>(buffer, buffer+lenght), ElementsAreArray({0x86 , 0x4F , 0xD2 , 0x6F , 0xB5 , 0x59 , 0xF7 , 0x5B}));
}

TEST(Z85Test, roundtrip) {
   const size_t BUFFER_SIZE = 100;
   char buffer[BUFFER_SIZE];
   uint8_t input[] = {11, 22, 33, 44};
   uint8_t output[sizeOfArry(input)];

   encode(input, sizeOfArry(input), buffer, BUFFER_SIZE);
   size_t lenght = decode(buffer, output, sizeOfArry(output));
   EXPECT_EQ(lenght, sizeOfArry(input));
   EXPECT_THAT(std::vector<uint8_t>(output, output+lenght), ElementsAreArray(input));
}



TEST(Z85Test, roundtrip_int16_t) {
   const size_t BUFFER_SIZE = 100;
   char buffer[BUFFER_SIZE];

   int16_t input1 = 4567;
   int16_t input2 = 4711;
   uint8_t inputArray[2*sizeof(int16_t)];
   Bt::Core::binaryAppend(input1,inputArray);
   Bt::Core::binaryAppend(input2,inputArray+sizeof(int16_t));
   uint8_t outputArray[sizeOfArry(inputArray)];

   encode(inputArray, sizeOfArry(inputArray), buffer, BUFFER_SIZE);
   log() << std::endl << "******" << std::endl << (const char*)buffer << std::endl << "******" << std::endl;
   size_t lenght = decode(buffer, outputArray, sizeOfArry(outputArray));
   log() << std::endl << "******" << std::endl;
   for(int i : outputArray) {
      log() << i << std::endl;
   }
   log() << "******" << std::endl;
   int16_t output1 = Bt::Core::binaryRead<int16_t>(outputArray);
   int16_t output2 = Bt::Core::binaryRead<int16_t>(outputArray+sizeof(int16_t));
   EXPECT_EQ(output1, input1);
   EXPECT_EQ(output2, input2);
}

TEST(Z85Test, roundtrip_negative_int16_t) {
   const size_t BUFFER_SIZE = 100;
   char buffer[BUFFER_SIZE];

   int16_t input1 = -1;
   int16_t input2 = -2;
   uint8_t inputArray[2*sizeof(int16_t)];
   Bt::Core::binaryAppend(input1,inputArray);
   Bt::Core::binaryAppend(input2,inputArray+sizeof(int16_t));
   uint8_t outputArray[sizeOfArry(inputArray)];

   encode(inputArray, sizeOfArry(inputArray), buffer, BUFFER_SIZE);
   log() << std::endl << "******" <<  strlen((const char*)buffer)  << std::endl << (const char*)buffer << std::endl << "******" << std::endl;
   size_t lenght = decode(buffer, outputArray, sizeOfArry(outputArray));
   log() << std::endl << "******" << std::endl;
   for(int i : outputArray) {
      log() << i << std::endl;
   }
   log() << "******" << std::endl;
   int16_t output1 = Bt::Core::binaryRead<int16_t>(outputArray);
   int16_t output2 = Bt::Core::binaryRead<int16_t>(outputArray+sizeof(int16_t));
   EXPECT_EQ(output1, input1);
   EXPECT_EQ(output2, input2);
}

TEST(Z85Test, roundtrip_negative_int16_t_borders) {
   const size_t BUFFER_SIZE = 100;
   char buffer[BUFFER_SIZE];

   int16_t inputs[] = {32767, 2345 , 1, 0, 0, -1 , -5432 , -32768};
   int16_t outputs[sizeOfArry(inputs)] = {0};

   uint8_t inputArray[sizeof(inputs)];

   for (int i = 0; i < sizeOfArry(inputs); ++i) {
      Bt::Core::binaryAppend(inputs[i],inputArray + i * sizeof(int16_t));
   }

   uint8_t outputArray[sizeof(inputs)];
   encode(inputArray, sizeOfArry(inputArray), buffer, BUFFER_SIZE);
   log() << std::endl << "******" <<  strlen((const char*)buffer) << std::endl << (const char*)buffer << std::endl << "******" << std::endl;
   size_t lenght = decode(buffer, outputArray, sizeOfArry(outputArray));
   log() << std::endl << "******" << lenght << std::endl;
   for(int i : outputArray) {
      log() << i << std::endl;
   }
   log() << "******" << std::endl;

   for (int i = 0; i < sizeOfArry(outputs); ++i) {
      outputs[i] = Bt::Core::binaryRead<int16_t>(outputArray + i * sizeof(int16_t));
   }

   EXPECT_THAT(std::vector<int16_t>(outputs, outputs+sizeOfArry(outputs)), ElementsAreArray(inputs));
}



} // namespace Z85
} // namespace Encoding
} // namespace Bt
