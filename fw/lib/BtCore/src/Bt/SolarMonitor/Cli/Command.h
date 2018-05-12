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

typedef std::function<void(Stream& pStream, int pArgc, char* pArgv[])> Command;

} // namespace Cli
} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_Cli_Command__h
