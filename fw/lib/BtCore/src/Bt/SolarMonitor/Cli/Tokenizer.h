//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::Cli::Tokenizer
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_Cli_Tokenizer__h
#define INC__Bt_SolarMonitor_Cli_Tokenizer__h

#include <functional>

namespace Bt {
namespace SolarMonitor {
namespace Cli {

class Tokenizer
{
   public:
      typedef std::function<void(int pArgc, char* pArgv[])> Consumer;

      Tokenizer(Consumer pConsumer);
      Tokenizer(const Tokenizer&) = delete;
      Tokenizer& operator=(const Tokenizer&) = delete;
      ~Tokenizer();

      void consume(char* pCmdline);

   private:
      static const size_t ARGS_SIZE = 10;
      Consumer mConsumer;
      
};

} // namespace Cli
} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_Cli_Tokenizer__h
