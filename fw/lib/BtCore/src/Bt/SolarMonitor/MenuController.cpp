//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::MenuController
//
//*************************************************************************************************

#include "Bt/SolarMonitor/MenuController.h"

namespace Bt {
namespace SolarMonitor {

MenuController::MenuController() :mStateFunction(&MenuController::idle) {

}

MenuController::~MenuController() {

}

void MenuController::wakeUp() {
   mStateFunction = &MenuController::active;
   mActiveTimer = Core::Timer(60*1000);
}

Core::Scheduling MenuController::workcycle() {
   return (this->*mStateFunction)();
}

Core::Scheduling MenuController::idle() {
   return Core::Scheduling::never();
}

Core::Scheduling MenuController::active() {
   if(mActiveTimer.expired()) {
      mStateFunction = &MenuController::idle;
      return Core::Scheduling::never();
   }
   return Core::Scheduling::immediately();
}


} // namespace SolarMonitor
} // namespace Bt
