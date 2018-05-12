//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::Cli::Command
//
//*************************************************************************************************

#include "Bt/SolarMonitor/Cli/Command.h"

namespace Bt {
namespace SolarMonitor {
namespace Cli {

Command::Command(const char* pName, std::function<void(Stream& pStream, int pArgc, char* pArgv[])> pExecutor)
: mName(pName), mExecutor(pExecutor)  {

}

Command::~Command() {

}

void Command::execute(Stream& pStream, int pArgc, char* pArgv[]) {
   mExecutor(pStream, pArgc, pArgv);
}


} // namespace Cli
} // namespace SolarMonitor
} // namespace Bt
