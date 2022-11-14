//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::Sleep
//
//*************************************************************************************************

#include "Bt/Core/Sleep.h"

#include <Bt/Core/Platform.h>
#include <Bt/Core/Log.h>

namespace Bt {
namespace Core {

void msSleep(unsigned long ms) {
   unsigned long start = millis();
   while(millis() - start <  ms) {
      __WFI();
   }
}

} // namespace Core
} // namespace Bt
