//*************************************************************************************************
//
//  BITTAILOR.CH  -  Bt::Core::I_SchedulingListener
//
//*************************************************************************************************

#ifndef INC__Bt_Core_I_SchedulingListener__h
#define INC__Bt_Core_I_SchedulingListener__h

#include "Bt/Core/IntrusiveList.h"

namespace Bt {
namespace Core {

class I_SchedulingListener : public IntrusiveList<I_SchedulingListener>::Element {
   public:
      virtual ~I_SchedulingListener() {}

      virtual void beforeStopModeSleep() = 0;
      virtual void afterStopModeSleep(bool pWakeUpPinState) = 0;
};

} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_I_SchedulingListener__h
