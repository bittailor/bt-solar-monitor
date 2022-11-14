//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::Cli::CommandRepository
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_Cli_CommandRepository__h
#define INC__Bt_SolarMonitor_Cli_CommandRepository__h

#undef min
#undef max
#undef swap

#include <map>
#include <functional>
#include <Bt/Core/Platform.h>
#include <Bt/Core/StrCmp.h>
#include <Bt/SolarMonitor/Cli/Command.h>

namespace Bt {
namespace SolarMonitor {
namespace Cli {

class CommandRepository
{
   private:
      typedef std::map<const char*, Command, Bt::Core::StrCmp> Commands;
   public:

      CommandRepository();
      CommandRepository(const CommandRepository&) = delete;
      CommandRepository& operator=(const CommandRepository&) = delete;
      ~CommandRepository();

      Commands::const_iterator begin() const { return mCommands.begin(); }
      Commands::const_iterator end() const { return mCommands.end(); }

      void add(const char* pName, const Command& pCommand);
      Command* lookup(const char* pName);

   private:
      

      Commands mCommands;

};

} // namespace Cli
} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_Cli_CommandRepository__h
