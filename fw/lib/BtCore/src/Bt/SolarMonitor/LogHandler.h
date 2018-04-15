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
      LogHandler();
      LogHandler(const LogHandler&) = delete;
      LogHandler& operator=(const LogHandler&) = delete;
      ~LogHandler();

      void disable();
      void enable();

      void changeLevel(LogLevel pLevel);

   private:
      LogLevel mLevel;
      PlacementPointer<Serial1LogHandler> mSerial1LogHandler;
      
};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_LogHandler__h
