//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Sensors::VeDirect::CommandExecutor
//
//*************************************************************************************************

#include "Bt/Sensors/VeDirect/CommandExecutor.h"

#include "Bt/Core/Log.h"
#include "Bt/Sensors/VeDirect/Utility.h"


namespace Bt {
namespace Sensors {
namespace VeDirect {
namespace {
   void logRead(int pChar){
      if(pChar >= 0 && pChar < 255 ) {
         BT_CORE_LOG_DEBUG("  c = %c [%d]", pChar, pChar);
      } else {
         BT_CORE_LOG_DEBUG("  c = %d", pChar);
      }
   }
}

CommandExecutor::CommandExecutor(Stream& pStream, const char* pCommand)
:mStream(pStream), mCommand(pCommand), mBuffer{0},mBufferIndex(0){

}

CommandExecutor::~CommandExecutor() {

}

const char* CommandExecutor::execute() {
   mBuffer[0] = 0;
   mBufferIndex = 0;
   mTimer = Bt::Core::Timer(200);
   mStateFunction = &CommandExecutor::waitForStartCharacter;
   flush();
   mStream.println(mCommand);
   State state = State::CONTINUE;
   while(state == State::CONTINUE){
      if(mTimer.expired()) { return nullptr; }
      state = (this->*mStateFunction)();
   };
   if(state == SUCCESS) {
      return mBuffer;
   }
   return nullptr;
}

void CommandExecutor::flush() {
   BT_CORE_LOG_DEBUG("flush ...");
   while(mStream.available()){
      int c = mStream.read();
      BT_CORE_LOG_DEBUG("  %c",c);
      if(mTimer.expired()) {
         return;
      }
   }
   BT_CORE_LOG_DEBUG("done");
}

CommandExecutor::State CommandExecutor::waitForStartCharacter() {
   int c = mStream.read();
   if(c == ':') {
      mBuffer[mBufferIndex] = c;
      mBufferIndex++;
      mStateFunction = &CommandExecutor::readUntilEndCharacter;
   } else {
      logRead(c);
   }
   return State::CONTINUE;
}

CommandExecutor::State CommandExecutor::readUntilEndCharacter() {
   int c = mStream.read();
   if(c == '\n') {
      mBuffer[mBufferIndex] = 0;
      BT_CORE_LOG_DEBUG("response => %s", mBuffer);
      mStateFunction = &CommandExecutor::checkMatch;
   } else if (c >= 0 && c < 255) {
      BT_CORE_LOG_DEBUG(" append %c at [%zd]", c, mBufferIndex);
      mBuffer[mBufferIndex] = c;
      mBufferIndex++;
      if(mBufferIndex >= BUFFER_SIZE-1) {
         BT_CORE_LOG_ERROR("CommandExecutor buffer full");
         return State::FAIL;
      }
   } else {
      logRead(c);
   }
   return State::CONTINUE;
}

CommandExecutor::State CommandExecutor::checkMatch() {
   int prefixLength = strlen(mCommand)-2;
   if(strncmp(mCommand, mBuffer, prefixLength) == 0) {
      BT_CORE_LOG_DEBUG("match [%d] command = %.*s response = %.*s ", prefixLength, prefixLength, mCommand, prefixLength, mBuffer);
      mStateFunction = &CommandExecutor::checkCheckSum;
      return State::CONTINUE;
   }

   BT_CORE_LOG_DEBUG("no match [%d] command = %.*s response = %.*s ", prefixLength, prefixLength, mCommand, prefixLength, mBuffer);
   mStateFunction = &CommandExecutor::waitForStartCharacter;
   mBuffer[0] = 0;
   mBufferIndex = 0;
   return State::CONTINUE;
}

CommandExecutor::State CommandExecutor::checkCheckSum() {
   size_t lenghtWithoutCheckSum = strlen(mBuffer)-2;
   uint8_t calculated = checksum(mBuffer,lenghtWithoutCheckSum);
   uint8_t received = hexToByte(mBuffer+lenghtWithoutCheckSum);

   if(calculated == received) {
      BT_CORE_LOG_DEBUG("checksum match => SUCCESS");
      return State::SUCCESS;
   }

   BT_CORE_LOG_DEBUG("checksum mismatch calculated=0x%X received=0x%X ", calculated, received);
   mBuffer[0] = 0;
   mBufferIndex = 0;
   return State::FAIL;
}



} // namespace VeDirect
} // namespace Sensors
} // namespace Bt
