//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::Cli::CliController
//
//*************************************************************************************************

#include "Bt/SolarMonitor/Cli/CliController.h"

#include <Bt/Core/Log.h>

namespace Bt {
namespace SolarMonitor {
namespace Cli {

CliController::CliController(Stream& pStream) : mBufferIndex(0), mStateFunction(&CliController::idle), mStream(pStream) {

}

CliController::~CliController() {

}

Core::Scheduling CliController::workcycle() {
   return (this->*mStateFunction)();
}

void CliController::beforeStopModeSleep() {

}

void CliController::afterStopModeSleep(bool pWakeUpPinState) {
   if(pWakeUpPinState) {
      BT_CORE_LOG_INFO("CliController WakeUp => listening");
      mActiveTimer = Core::Timer(20*1000);
      mStateFunction = &CliController::listening;
   }
}


Core::Scheduling CliController::idle() {
   return Core::Scheduling::never();
}

Core::Scheduling CliController::listening() {
   Bt::Core::Timer timer(200);
   while(mStream.available() && !timer.expired()) {
      mActiveTimer = Core::Timer(20*1000);
      char c = mStream.read();
      mStream.print(c);
      if(c == '\r' || c == '\n' ) {
         if(mBufferIndex != 0 ) {
            execute(mBuffer.data());
         }
         mBufferIndex = 0;
      }
      else if(c == 0x8 /*BS*/ || c == 0x7F /*DEL*/ ) {
         if(mBufferIndex > 0) {
            mBufferIndex--;
         }
      } else {
         mBuffer[mBufferIndex++] = c;
         if(mBufferIndex >= mBuffer.size()) {
            mBufferIndex = 0;
         }
      }
      mBuffer[mBufferIndex] = 0;
   }
   if(mActiveTimer.expired()) {
      BT_CORE_LOG_INFO("CliController ActiveTimer.expired => idle");
      mStateFunction = &CliController::idle;
   }
   return Core::Scheduling::immediately();
}

void CliController::execute(const char* cmdline) {
   mStream.printlnf("cmd> '%s'", cmdline);
   if(strcmp(cmdline, "exit") == 0) {
      BT_CORE_LOG_INFO("CliController exit => idle");
      mStateFunction = &CliController::idle;
   }
   if(strcmp(cmdline, "dfu") == 0) {
      BT_CORE_LOG_INFO("CliController dfu");
      System.dfu();
   }
}

} // namespace Cli
} // namespace SolarMonitor
} // namespace Bt
