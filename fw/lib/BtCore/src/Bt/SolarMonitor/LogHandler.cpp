//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::LogHandler
//
//*************************************************************************************************

#include "Bt/SolarMonitor/LogHandler.h"

#include <map>
#include <Bt/Core/StrCmp.h>

namespace Bt {
namespace SolarMonitor {
namespace {
   std::map<const char*,LogLevel,Bt::Core::StrCmp> sLevels = {
      {"all", LOG_LEVEL_ALL},
      {"trace", LOG_LEVEL_TRACE},
      {"info", LOG_LEVEL_INFO},
      {"warn", LOG_LEVEL_WARN},
      {"error", LOG_LEVEL_ERROR},
      {"panic", LOG_LEVEL_PANIC},
      {"none", LOG_LEVEL_NONE}
   };
}

LogHandler::LogHandler(LogLevel pLevel, std::function<spark::LogHandler*(LogLevel pLevel)> pFactory) 
: mLevel(pLevel)
, mFactory(pFactory ? pFactory : [](LogLevel pLevel){ return new Serial1LogHandler(115200, pLevel);}) {
   enable();
}

LogHandler::~LogHandler() {
   disable();
}

void LogHandler::disable() {
   if(mLogHandler != nullptr) {
      delete mLogHandler;
      mLogHandler = nullptr;
   }
}

void LogHandler::enable() {
   if(mLogHandler == nullptr) {
      mLogHandler = mFactory(mLevel);
   }
}

void LogHandler::changeLevel(LogLevel pLevel) {
   if(mLevel != pLevel) {
      mLevel = pLevel;
      if(mLogHandler != nullptr) {
         disable();
         enable();
      }
   }
}

void LogHandler::changeLevel(Stream& pStream, int pArgc, char* pArgv[]) {
   if(pArgc < 2 ) {
      pStream.printlnf("cli-e>%s: need a level as second argument", pArgv[0]);
   }
   const char* level = pArgv[1];
   auto iter = sLevels.find(level);
   if(iter != sLevels.end()) {
      changeLevel(iter->second);
      pStream.printlnf("cli-i> set log level to %s", iter->first);
      return;
   }
   pStream.printlnf("cli-e> unknown level %s", level);
   for(auto& l : sLevels) {
      pStream.printlnf("cli-e> - %s", l.first);
   }
   return;
}

} // namespace SolarMonitor
} // namespace Bt
