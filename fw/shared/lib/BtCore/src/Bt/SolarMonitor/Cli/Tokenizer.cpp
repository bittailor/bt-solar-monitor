//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::Cli::Tokenizer
//
//*************************************************************************************************

#include "Bt/SolarMonitor/Cli/Tokenizer.h"

#include <string.h>

namespace Bt {
namespace SolarMonitor {
namespace Cli {

Tokenizer::Tokenizer(Consumer pConsumer) : mConsumer(pConsumer)  {

}

Tokenizer::~Tokenizer() {

}

void Tokenizer::consume(char* pCmdline) {

   size_t argc = 0;
   char* argv[ARGS_SIZE] = {nullptr};

   char* token = strtok(pCmdline, " ");
   while (token != nullptr) {
      if (argc < ARGS_SIZE) {
         argv[argc++] = token;
      }
      token = strtok(NULL, " ");
   }

   if(mConsumer) {
      mConsumer(argc, argv);
   }
}

} // namespace Cli
} // namespace SolarMonitor
} // namespace Bt
