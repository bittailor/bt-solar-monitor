//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::Workcycle
//
//*************************************************************************************************

#include "Bt/Core/Workcycle.h"

#include <algorithm>
#include <Bt/Core/Platform.h>
#include <Bt/Core/Log.h>
#include <Bt/Core/Sleep.h>

namespace Bt {
namespace Core {

//-------------------------------------------------------------------------------------------------

Workcycle::Workcycle() {

}

//-------------------------------------------------------------------------------------------------

Workcycle::~Workcycle() {

}

//-------------------------------------------------------------------------------------------------

void Workcycle::add(I_Runnable& iRunnable) {
   mRunnables.pushBack(iRunnable);
}

//-------------------------------------------------------------------------------------------------

void Workcycle::remove(I_Runnable& iRunnable) {
   mRunnables.remove(iRunnable);
}

//-------------------------------------------------------------------------------------------------

void Workcycle::oneWorkcycle() {
   uint32_t timer = millis();
   BT_CORE_LOG_INFO(">>workcycle start");
   Scheduling nextScheduling = Scheduling::never();
   for (I_Runnable& runnable : mRunnables) {
      Scheduling runnableScheduling = runnable.workcycle();
      nextScheduling = std::min(runnableScheduling, nextScheduling);
   }
   timer = millis() - timer;
   BT_CORE_LOG_INFO("<<workcycle end took %lu ms => scheduling %s  [%lu]", timer, nextScheduling.typeString(), nextScheduling.delay());
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
         //Serial1.flush();
         System.sleep(A0, RISING, pScheduling.delay());
         return;
      }
      case Scheduling::NEVER : {
         return;
      }
   }
}


//-------------------------------------------------------------------------------------------------

} // namespace Core
} // namespace Bt
