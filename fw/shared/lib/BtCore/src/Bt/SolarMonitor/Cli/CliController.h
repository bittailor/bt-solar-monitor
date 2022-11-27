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
#include <Bt/SolarMonitor/Cli/Tokenizer.h>
#include <Bt/SolarMonitor/Cli/CommandRepository.h>

namespace Bt {
namespace SolarMonitor {
namespace Cli {

class CliController : public Core::I_Runnable , public Core::I_SchedulingListener
{
   public:
      CliController(USARTSerial& pUSARTSerial);
      CliController(const CliController&) = delete;
      CliController& operator=(const CliController&) = delete;
      ~CliController();

      void addCommand(const char* pName, const Command& pCommand);

      virtual Core::Scheduling workcycle();

      virtual void beforeStopModeSleep(SystemSleepConfiguration& pSleepConfiguration);
      virtual void afterStopModeSleep(SystemSleepWakeupReason pWakeUpReason);

   private:
      Core::Scheduling idle();
      Core::Scheduling listening();
      void consumeStream();
      void execute(char* cmdline);
      void execute(int pArgc, char* pArgv[]);
      
      typedef Core::Scheduling (CliController::*StateFunction)();
      static const size_t BUFFER_SIZE = 256;
      static const size_t ARGS_SIZE = 10;

      Core::Timer mActiveTimer;
      size_t mBufferIndex;
      std::array<char,BUFFER_SIZE> mBuffer;
      StateFunction mStateFunction;
      USARTSerial& mUSARTSerial;
      Tokenizer mTokenizer;
      CommandRepository mCommandRepository;


};

} // namespace Cli
} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_Cli_CliController__h
