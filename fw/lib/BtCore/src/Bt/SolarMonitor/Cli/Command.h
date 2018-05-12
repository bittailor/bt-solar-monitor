//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::Cli::Command
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_Cli_Command__h
#define INC__Bt_SolarMonitor_Cli_Command__h

#include <functional>
#include <Bt/Core/Platform.h>

namespace Bt {
namespace SolarMonitor {
namespace Cli {

class Command
{
   public:
      Command(const char* pName, std::function<void(Stream& pStream, int pArgc, char* pArgv[])> pExecutor);
      Command(const Command&) = default;
      Command& operator=(const Command&) = default;
      ~Command();

      const char* name() const {return mName;}
      void execute(Stream& pStream, int pArgc, char* pArgv[]);

   private:
      const char* mName;
      std::function<void(Stream& pStream, int pArgc, char* pArgv[])> mExecutor;
      
};

} // namespace Cli
} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_Cli_Command__h
