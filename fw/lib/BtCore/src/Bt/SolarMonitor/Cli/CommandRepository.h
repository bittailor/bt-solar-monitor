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
#include <Bt/SolarMonitor/Cli/Command.h>

namespace Bt {
namespace SolarMonitor {
namespace Cli {

class CommandRepository
{
      struct StrCmp
      {
            bool operator()(char const *a, char const *b)
            {
               return strcmp(a, b) < 0;
            }
      };
      typedef std::map<const char*, Command, StrCmp> Commands;
   public:

      CommandRepository();
      CommandRepository(const CommandRepository&) = delete;
      CommandRepository& operator=(const CommandRepository&) = delete;
      ~CommandRepository();

      Commands::const_iterator begin() const { return mCommands.begin(); }
      Commands::const_iterator end() const { return mCommands.end(); }

      void add(const Command& pCommand);

      Command* lookup(const char* pName);

   private:
      

      Commands mCommands;

};

} // namespace Cli
} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_Cli_CommandRepository__h
