//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::Cli::CliController
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_Cli_CliController__h
#define INC__Bt_SolarMonitor_Cli_CliController__h

#include <Bt/Core/Platform.h>
#include <Bt/Core/I_Runnable.h>
#include <Bt/Core/Timer.h>
#include <Bt/Core/I_SchedulingListener.h>

namespace Bt {
namespace SolarMonitor {
namespace Cli {

class CliController : public Core::I_Runnable , public Core::I_SchedulingListener
{
   public:
      CliController(Stream& pStream);
      CliController(const CliController&) = delete;
      CliController& operator=(const CliController&) = delete;
      ~CliController();

      virtual Core::Scheduling workcycle();

      virtual void beforeStopModeSleep();
      virtual void afterStopModeSleep(bool pWakeUpPinState);

   private:
      Core::Scheduling idle();
      Core::Scheduling listening();
      void execute(const char* cmdline);
      
      typedef Core::Scheduling (CliController::*StateFunction)();

      Core::Timer mActiveTimer;
      size_t mBufferIndex;
      std::array<char,256> mBuffer;
      StateFunction mStateFunction;
      Stream& mStream;

};

} // namespace Cli
} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_Cli_CliController__h
