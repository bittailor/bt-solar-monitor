//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::PeriodicCallback
//
//*************************************************************************************************

#include "Bt/Core/PeriodicCallback.h"

#include <Bt/Core/Singleton.h>

namespace Bt {
namespace Core {
namespace {

uint32_t timeNow(PeriodicCallback::TimeUnit pTimeUnit) {
   switch(pTimeUnit){
      case PeriodicCallback::TimeUnit::MILLIS : return Singleton<I_Time>::instance()->milliseconds();
      case PeriodicCallback::TimeUnit::SECONDS : return Singleton<I_Time>::instance()->seconds();
   }
   return 0;
}

Scheduling scheduling(PeriodicCallback::TimeUnit pTimeUnit, uint32_t pDelay) {
   switch(pTimeUnit){
      case PeriodicCallback::TimeUnit::MILLIS : return Scheduling::inMillis(pDelay);
      case PeriodicCallback::TimeUnit::SECONDS : return Scheduling::inSeconds(pDelay);
   }
   return Scheduling::immediately();
}


}


//-------------------------------------------------------------------------------------------------

PeriodicCallback::PeriodicCallback(TimeUnit pTimeUnit, uint32_t pPeriod, std::function<void()> pCallback)
: mTimeUnit(pTimeUnit), mPeriod(pPeriod), mStartTime(timeNow(pTimeUnit)), mCallback(pCallback){
}

//-------------------------------------------------------------------------------------------------

Scheduling PeriodicCallback::workcycle() {
   uint32_t now = timeNow(mTimeUnit);
   uint32_t diff = now - mStartTime;
   if (diff >= mPeriod) {
      mStartTime = now;
      if(mCallback) {
         mCallback();
      }
      return scheduling(mTimeUnit, mPeriod);
   }
   return scheduling(mTimeUnit, mPeriod - diff);
}
//-------------------------------------------------------------------------------------------------

} // namespace Core
} // namespace Bt
