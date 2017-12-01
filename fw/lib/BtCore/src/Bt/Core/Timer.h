//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::Timer
//
//*************************************************************************************************

#ifndef INC__Bt_Core_Timer__h
#define INC__Bt_Core_Timer__h

#include <stdint.h>
#include "Bt/Core/Platform.h"

namespace Bt {
namespace Core {

class Timer {
   public:
      Timer() : mInterval(0), mStart(millis()) {
      }

      Timer(int ms) : mInterval(ms), mStart(millis()) {
      }

      bool expired() {
         return (millis() - mStart) >= mInterval;
      }

      void expand(int ms) {
         mInterval = mInterval + ms;
      }

   private:
      uint32_t mInterval;
      uint32_t mStart;
};

} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_Timer__h
