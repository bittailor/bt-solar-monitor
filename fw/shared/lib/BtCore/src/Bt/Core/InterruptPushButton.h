//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::InterruptPushButton
//
//*************************************************************************************************

#ifndef INC__Bt_Core_InterruptPushButton__h
#define INC__Bt_Core_InterruptPushButton__h

#include <stdint.h>
#include <Bt/Core/Timer.h>
#include <Bt/Core/I_Runnable.h>
#include <Bt/Core/I_SchedulingListener.h>

namespace Bt {
namespace Core {

class InterruptPushButton : public I_Runnable, public I_SchedulingListener
{
   public:
      InterruptPushButton(uint16_t pPin, std::function<void()> pOnClick = std::function<void()>());
      InterruptPushButton(const InterruptPushButton&) = delete;
      InterruptPushButton& operator=(const InterruptPushButton&) = delete;
      ~InterruptPushButton();

      void begin();
   
      virtual Scheduling workcycle();

      virtual void beforeStopModeSleep(SystemSleepConfiguration& pSleepConfiguration);
      virtual void afterStopModeSleep(SystemSleepWakeupReason pWakeUpReason);


   private:
      void interruptHandler();

      uint16_t mPin;
      std::function<void()> mOnClick;
      bool mPressed;
      bool mFire;
      Timer mTimer;
      Timer mAwakeTimer;
};

} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_InterruptPushButton__h
