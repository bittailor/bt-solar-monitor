//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::Workcycle
//
//*************************************************************************************************

#include "Bt/Core/Workcycle.h"

#include <algorithm>
#include <inttypes.h>
#include <Bt/Core/Platform.h>
#include <Bt/Core/Log.h>
#include <Bt/Core/Sleep.h>

namespace Bt {
namespace Core {

//-------------------------------------------------------------------------------------------------

Workcycle::Workcycle(uint16_t pWakeUpPin) : mWakeUpPin(pWakeUpPin) {

}

//-------------------------------------------------------------------------------------------------

Workcycle::~Workcycle() {

}

//-------------------------------------------------------------------------------------------------

void Workcycle::begin() {
   pinMode(mWakeUpPin, INPUT_PULLDOWN);
}

//-------------------------------------------------------------------------------------------------

void Workcycle::add(I_Runnable& pRunnable) {
   mRunnables.pushBack(pRunnable);
}

//-------------------------------------------------------------------------------------------------

void Workcycle::remove(I_Runnable& pRunnable) {
   mRunnables.remove(pRunnable);
}

//-------------------------------------------------------------------------------------------------

void Workcycle::addSchedulingListener(I_SchedulingListener& pSchedulingListener) {
   mSchedulingListeners.pushBack(pSchedulingListener);
}

//-------------------------------------------------------------------------------------------------

void Workcycle::removeSchedulingListener(I_SchedulingListener& pSchedulingListener) {
   mSchedulingListeners.remove(pSchedulingListener);
}

//-------------------------------------------------------------------------------------------------

void Workcycle::oneWorkcycle() {
   uint32_t timer = millis();
   // BT_CORE_LOG_DEBUG(">>workcycle start");
   Scheduling nextScheduling = Scheduling::never();
   for (I_Runnable& runnable : mRunnables) {
      Scheduling runnableScheduling = runnable.workcycle();
      nextScheduling = std::min(runnableScheduling, nextScheduling);
   }
   timer = millis() - timer;
   // BT_CORE_LOG_DEBUG("<<workcycle end took %" PRIu32 " ms => scheduling %s  [%" PRIu32 "]", timer, nextScheduling.typeString(), nextScheduling.delay());
   scheduling(nextScheduling);
}

//-------------------------------------------------------------------------------------------------

void Workcycle::scheduling(Scheduling pScheduling) {
   switch(pScheduling.type()) {
      case Scheduling::IMMEDIATELY : {
         return;
      }
      case Scheduling::MILLIS_DELAY : {
         msSleep(pScheduling.delay());
         return;
      }
      case Scheduling::SECONDS_DELAY : {
         beforeStopModeSleep();
         System.sleep(mWakeUpPin, RISING, pScheduling.delay());
         afterStopModeSleep();
         return;
      }
      case Scheduling::NEVER : {
         beforeStopModeSleep();
         System.sleep(mWakeUpPin, RISING);
         afterStopModeSleep();
         return;
      }
   }
}

//-------------------------------------------------------------------------------------------------

void Workcycle::beforeStopModeSleep()
{
   for (I_SchedulingListener& listener : mSchedulingListeners) {
      listener.beforeStopModeSleep();
   }
}

//-------------------------------------------------------------------------------------------------

void Workcycle::afterStopModeSleep()
{
   bool wakeUpPin = pinReadFast(mWakeUpPin) == HIGH;
   for (I_SchedulingListener& listener : mSchedulingListeners) {
      listener.afterStopModeSleep(wakeUpPin);
   }
}

//-------------------------------------------------------------------------------------------------

} // namespace Core
} // namespace Bt
