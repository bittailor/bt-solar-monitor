//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::Ui::ViewsController
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_Ui_ViewsController__h
#define INC__Bt_SolarMonitor_Ui_ViewsController__h

#include <Bt/Core/I_Runnable.h>
#include <Bt/Core/I_SchedulingListener.h>
#include <Bt/Core/Timer.h>

namespace Bt {
namespace SolarMonitor {
namespace Ui {

class ViewsController : public Core::I_Runnable , public Core::I_SchedulingListener
{
   public:
      ViewsController();
      ViewsController(const ViewsController&) = delete;
      ViewsController& operator=(const ViewsController&) = delete;
      ~ViewsController();

      void wakeUp();
      void select();
      void next();
      void previous();


      virtual Core::Scheduling workcycle();

      virtual void beforeStopModeSleep();
      virtual void afterStopModeSleep(bool pWakeUpPinState);

   private:
      typedef Core::Scheduling (ViewsController::*StateFunction)();
      
      Core::Scheduling idle();
      Core::Scheduling active();

      StateFunction mStateFunction;
      Core::Timer mActiveTimer;
};

} // namespace Ui
} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_Ui_ViewsController__h
