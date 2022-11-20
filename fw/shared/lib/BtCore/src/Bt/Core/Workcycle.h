//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::Workcycle
//
//*************************************************************************************************

#ifndef INC__Bt_Core_Workcycle__h
#define INC__Bt_Core_Workcycle__h

#include <vector>
#include "Bt/Core/I_SchedulingListener.h"
#include "Bt/Core/I_Workcycle.h"

namespace Bt {
namespace Core {

class Workcycle : public I_Workcycle
{
   public:
      Workcycle(uint16_t pWakeUpPin);
      ~Workcycle();

      void begin();

      virtual void add(I_Runnable& pRunnable);
      virtual void remove(I_Runnable& pRunnable);

      virtual void addSchedulingListener(I_SchedulingListener& pSchedulingListener);
      virtual void removeSchedulingListener(I_SchedulingListener& pSchedulingListener);


      void oneWorkcycle();

   private:
        // Constructor to prohibit copy construction.
      Workcycle(const Workcycle&);

      // Operator= to prohibit copy assignment
      Workcycle& operator=(const Workcycle&);

      void scheduling(Scheduling pScheduling);
      void beforeStopModeSleep();
      void afterStopModeSleep(SystemSleepWakeupReason pWakeUpReason);

      typedef Core::IntrusiveList<I_Runnable> Runnables;
      typedef Core::IntrusiveList<I_SchedulingListener> SchedulingListeners;

      Runnables mRunnables;
      SchedulingListeners mSchedulingListeners;
      uint16_t mWakeUpPin;
};


} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_Workcycle__h
