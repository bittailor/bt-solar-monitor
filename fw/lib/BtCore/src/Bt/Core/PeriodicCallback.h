//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::PeriodicCallback
//
//*************************************************************************************************

#ifndef INC__Bt_Core_PeriodicCallback__hpp
#define INC__Bt_Core_PeriodicCallback__hpp

#include <functional>
#include <Bt/Core/I_Runnable.h>
#include "Bt/Core/I_Time.hpp"

namespace Bt {
namespace Core {

class PeriodicCallback : public I_Runnable {
   public:
      enum TimeUnit {SECONDS, MILLIS};

      PeriodicCallback(TimeUnit pTimeUnit, uint32_t pPeriod, std::function<void()> pCallback);
      virtual Scheduling workcycle();

   private:
      // Constructor to prohibit copy construction.
      PeriodicCallback(const PeriodicCallback&);

      // Operator= to prohibit copy assignment
      PeriodicCallback& operator=(const PeriodicCallback&);

      TimeUnit mTimeUnit;
      const uint32_t mPeriod;
      uint32_t mStartTime;
      std::function<void()> mCallback;
};

} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_PeriodicCallback__hpp
