//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::LogHandler
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_LogHandler__h
#define INC__Bt_SolarMonitor_LogHandler__h

#include <Bt/Core/Platform.h>
#include <Bt/Core/PlacementPointer.h>

using Bt::Core::PlacementPointer;

namespace Bt {
namespace SolarMonitor {

class LogHandler
{
   public:
      LogHandler(LogLevel pLevel = LOG_LEVEL_INFO, std::function<spark::LogHandler*(LogLevel pLevel)> pFactory = nullptr);
      LogHandler(const LogHandler&) = delete;
      LogHandler& operator=(const LogHandler&) = delete;
      ~LogHandler();

      void disable();
      void enable();

      void changeLevel(LogLevel pLevel);
      void changeLevel(Stream& pStream, int pArgc, char* pArgv[]);

   private:
      LogLevel mLevel;
      std::function<spark::LogHandler*(LogLevel pLevel)> mFactory;
      spark::LogHandler* mLogHandler;
      
};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_LogHandler__h
