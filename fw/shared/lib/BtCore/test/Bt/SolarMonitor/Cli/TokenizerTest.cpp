//*************************************************************************************************
//
//  BITTAILOR.CH
//
//*************************************************************************************************

#include <gtest/gtest.h>

#include <vector>

#include "Bt/SolarMonitor/Cli/Tokenizer.h"



namespace Bt {
namespace SolarMonitor {
namespace Cli {

class TokenizerTest : public ::testing::Test {

   protected:

      TokenizerTest()
      : mArgc(0)
      , mTokenizer([this](int pArgc, char* pArgv[]){
         mArgc = pArgc;
         for (int i = 0; i < pArgc ; ++i) {
            mArgv.push_back(pArgv[i]);
         }
      }){
         mBuffer[0] = 0;
      }

      virtual void SetUp() {
      }

      virtual void TearDown() {

      }

      char mBuffer[256];
      int mArgc;
      std::vector<char*> mArgv;
      Tokenizer mTokenizer;


};


TEST_F(TokenizerTest, zeroArgCmd) {
   strcpy(mBuffer, "exit");
   mTokenizer.consume(mBuffer);
   EXPECT_EQ(1, mArgc);
   EXPECT_STREQ("exit", mArgv[0]);
}

TEST_F(TokenizerTest, oneArgCmd) {
   strcpy(mBuffer, "button up");
   mTokenizer.consume(mBuffer);
   EXPECT_EQ(2, mArgc);
   EXPECT_STREQ("button", mArgv[0]);
   EXPECT_STREQ("up", mArgv[1]);
}

TEST_F(TokenizerTest, twoArgCmd) {
   strcpy(mBuffer, "doit 1 2");
   mTokenizer.consume(mBuffer);
   EXPECT_EQ(3, mArgc);
   EXPECT_STREQ("doit", mArgv[0]);
   EXPECT_STREQ("1", mArgv[1]);
   EXPECT_STREQ("2", mArgv[2]);
}

TEST_F(TokenizerTest, NineArgCmd) {
   strcpy(mBuffer, "doit 1 2 3 4 5 6 7 8 9");
   mTokenizer.consume(mBuffer);
   EXPECT_EQ(10, mArgc);
   EXPECT_STREQ("doit", mArgv[0]);
   EXPECT_STREQ("1", mArgv[1]);
   EXPECT_STREQ("2", mArgv[2]);
   EXPECT_STREQ("3", mArgv[3]);
   EXPECT_STREQ("4", mArgv[4]);
   EXPECT_STREQ("5", mArgv[5]);
   EXPECT_STREQ("6", mArgv[6]);
   EXPECT_STREQ("7", mArgv[7]);
   EXPECT_STREQ("8", mArgv[8]);
   EXPECT_STREQ("9", mArgv[9]);
}

TEST_F(TokenizerTest, TooManyArgCmd) {
   strcpy(mBuffer, "doit 1 2 3 4 5 6 7 8 9 10 11 12 13");
   mTokenizer.consume(mBuffer);
   EXPECT_EQ(10, mArgc);
   EXPECT_STREQ("doit", mArgv[0]);
   EXPECT_STREQ("1", mArgv[1]);
   EXPECT_STREQ("2", mArgv[2]);
   EXPECT_STREQ("3", mArgv[3]);
   EXPECT_STREQ("4", mArgv[4]);
   EXPECT_STREQ("5", mArgv[5]);
   EXPECT_STREQ("6", mArgv[6]);
   EXPECT_STREQ("7", mArgv[7]);
   EXPECT_STREQ("8", mArgv[8]);
   EXPECT_STREQ("9", mArgv[9]);
}

TEST_F(TokenizerTest, NoCmd) {
   strcpy(mBuffer, "");
   mTokenizer.consume(mBuffer);
   EXPECT_EQ(0, mArgc);
}



} // namespace Cli
} // namespace SolarMonitor
} // namespace Bt
