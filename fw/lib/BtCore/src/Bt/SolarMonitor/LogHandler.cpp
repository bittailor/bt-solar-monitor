//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::LogHandler
//
//*************************************************************************************************

#include "Bt/SolarMonitor/LogHandler.h"

namespace Bt {
namespace SolarMonitor {

LogHandler::LogHandler() : mLevel(LOG_LEVEL_INFO) {
   enable();
}

LogHandler::~LogHandler() {
   if(mSerial1LogHandler) {
      mSerial1LogHandler.dispose();
   }
}

void LogHandler::disable() {
   if(mSerial1LogHandler) {
      mSerial1LogHandler.dispose();
   }
}

void LogHandler::enable() {
   if(!mSerial1LogHandler) {
      mSerial1LogHandler.create(115200, mLevel);
   }
}

void LogHandler::changeLevel(LogLevel pLevel) {
   if(mLevel != pLevel) {
      mLevel = pLevel;
      if(mSerial1LogHandler) {
         disable();
         enable();
      }
   }
}

} // namespace SolarMonitor
} // namespace Bt
