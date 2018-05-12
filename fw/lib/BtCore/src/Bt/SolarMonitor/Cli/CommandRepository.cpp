//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::Cli::CommandRepository
//
//*************************************************************************************************

#include "Bt/SolarMonitor/Cli/CommandRepository.h"

namespace Bt {
namespace SolarMonitor {
namespace Cli {

CommandRepository::CommandRepository() {

}

CommandRepository::~CommandRepository() {

}

void CommandRepository::add(const Command& pCommand)  {
   mCommands.insert(Commands::value_type(pCommand.name(), pCommand));
}

Command* CommandRepository::lookup(const char* pName) {
   auto iter = mCommands.find(pName);
   if(iter != mCommands.end()) {
      return &iter->second;
   }
   return nullptr;
}

} // namespace Cli
} // namespace SolarMonitor
} // namespace Bt
