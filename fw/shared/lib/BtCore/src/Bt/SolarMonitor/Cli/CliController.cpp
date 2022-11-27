//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::Cli::CliController
//
//*************************************************************************************************

#include "Bt/SolarMonitor/Cli/CliController.h"

#include <inttypes.h>
#include <Bt/Core/Log.h>

namespace Bt {
namespace SolarMonitor {
namespace Cli {

CliController::CliController(USARTSerial& pUSARTSerial)
: mBufferIndex(0), mStateFunction(&CliController::idle), mUSARTSerial(pUSARTSerial)
, mTokenizer([this](int pArgc, char* pArgv[]){
   execute(pArgc,pArgv);
}){

   mCommandRepository.add("exit", [this](Stream& pStream, int pArgc, char* pArgv[]){
      BT_CORE_LOG_INFO("CliController exit cmd => idle");
      mStateFunction = &CliController::idle;
   });

   mCommandRepository.add("help", [this](Stream& pStream, int pArgc, char* pArgv[]){
      for(auto command : mCommandRepository) {
         pStream.printlnf(" - %s", command.first);
      }
   });

   mCommandRepository.add("dfu", [](Stream& pStream, int pArgc, char* pArgv[]){
      pStream.printlnf("enter DFU mode");
      pStream.flush();
      System.dfu();
   });

   mCommandRepository.add("reset", [](Stream& pStream, int pArgc, char* pArgv[]){
         pStream.printlnf("reset !");
         pStream.flush();
         System.reset();
   });

   mCommandRepository.add("memory", [](Stream& pStream, int pArgc, char* pArgv[]){
      pStream.printlnf("freeMemory = %" PRIu32, System.freeMemory());
   });

}

CliController::~CliController() {

}


void CliController::addCommand(const char* pName, const Command& pCommand) {
   mCommandRepository.add(pName, pCommand);
}

Core::Scheduling CliController::workcycle() {
   return (this->*mStateFunction)();
}

void CliController::beforeStopModeSleep(SystemSleepConfiguration& pSleepConfiguration) {
   pSleepConfiguration.usart(mUSARTSerial);
}

void CliController::afterStopModeSleep(SystemSleepWakeupReason pWakeUpReason) {
   if(pWakeUpReason == SystemSleepWakeupReason::BY_USART) {
      mUSARTSerial.printlnf("*** CLI => listening ***");
      mActiveTimer = Core::Timer(20*1000);
      mStateFunction = &CliController::listening;
   }
}


Core::Scheduling CliController::idle() {
   consumeStream();
   return Core::Scheduling::never();
}

Core::Scheduling CliController::listening() {
   consumeStream();
   if(mActiveTimer.expired()) {
      mUSARTSerial.printlnf("*** CLI => exit ***");
      mStateFunction = &CliController::idle;
   }
   return Core::Scheduling::immediately();
}

void CliController::consumeStream() {
   Bt::Core::Timer timer(500);
   while(mUSARTSerial.available() && !timer.expired()) {
      mActiveTimer = Core::Timer(30*1000);
      char c = mUSARTSerial.read();
      mUSARTSerial.print(c);
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
            BT_CORE_LOG_ERROR("CliController buffer full! => flush");
            mBufferIndex = 0;
         }
      }
      mBuffer[mBufferIndex] = 0;
   }
}

void CliController::execute(char* cmdline) {
   mUSARTSerial.printlnf("cli-i> cmdline = '%s'>", cmdline);
   mTokenizer.consume(cmdline);
}

void CliController::execute(int pArgc, char* pArgv[]) {
   if(pArgc <= 0) {
      return;
   }
   Command* command = mCommandRepository.lookup(pArgv[0]);
   if(command == nullptr) {
      mUSARTSerial.printlnf("cli-e> cmd '%s' not found!>", pArgv[0]);
      return;
   }
   (*command)(mUSARTSerial, pArgc, pArgv);
}

} // namespace Cli
} // namespace SolarMonitor
} // namespace Bt
