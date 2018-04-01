//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::MenuController
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_MenuController__h
#define INC__Bt_SolarMonitor_MenuController__h

#include <Bt/Core/I_Runnable.h>
#include <Bt/Core/Timer.h>

namespace Bt {
namespace SolarMonitor {

class MenuController : public Core::I_Runnable
{
   public:
      MenuController();
      MenuController(const MenuController&) = delete;
      MenuController& operator=(const MenuController&) = delete;
      ~MenuController();

      void wakeUp();
      void select();
      void next();
      void previous();


      virtual Core::Scheduling workcycle();

   private:
      typedef Core::Scheduling (MenuController::*StateFunction)();
      
      Core::Scheduling idle();
      Core::Scheduling active();

      StateFunction mStateFunction;
      Core::Timer mActiveTimer;
};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_MenuController__h
