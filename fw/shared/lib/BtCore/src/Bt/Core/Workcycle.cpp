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
         SystemSleepConfiguration config;
         config.mode(SystemSleepMode::STOP)
               .gpio(mWakeUpPin, RISING)
               .usart(Serial1)
               .duration(pScheduling.delay()*1000);;
         SystemSleepResult result = System.sleep(config);
         auto reason = result.wakeupReason();
         BT_CORE_LOG_INFO("wakeupReason = %d", asInteger(reason));
         afterStopModeSleep(reason);
         return;
      }
      case Scheduling::NEVER : {
         beforeStopModeSleep();
         SystemSleepConfiguration config;
         config.mode(SystemSleepMode::STOP)
               .gpio(mWakeUpPin, RISING)
               .usart(Serial1);
         SystemSleepResult result = System.sleep(config);
         auto reason = result.wakeupReason();
         BT_CORE_LOG_INFO("wakeupReason = %d", asInteger(reason));
         afterStopModeSleep(reason);
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

void Workcycle::afterStopModeSleep(SystemSleepWakeupReason pWakeUpReason)
{
   for (I_SchedulingListener& listener : mSchedulingListeners) {
      listener.afterStopModeSleep(pWakeUpReason);
   }
}

//-------------------------------------------------------------------------------------------------

} // namespace Core
} // namespace Bt
