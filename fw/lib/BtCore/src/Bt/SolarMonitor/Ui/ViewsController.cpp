//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::Ui::ViewsController
//
//*************************************************************************************************

#include "Bt/SolarMonitor/Ui/ViewsController.h"

#include <Bt/Core/Log.h>

namespace Bt {
namespace SolarMonitor {
namespace Ui {

ViewsController::ViewsController() :mStateFunction(&ViewsController::idle) {

}

ViewsController::~ViewsController() {

}

void ViewsController::wakeUp() {
   mStateFunction = &ViewsController::active;
   mActiveTimer = Core::Timer(20*1000);
}

void ViewsController::select() {

}

void ViewsController::next() {

}

void ViewsController::previous() {

}

Core::Scheduling ViewsController::workcycle() {
   return (this->*mStateFunction)();
}

void ViewsController::beforeStopModeSleep() {

}

void ViewsController::afterStopModeSleep(bool pWakeUpPinState) {
   if(pWakeUpPinState) {
      BT_CORE_LOG_INFO("ViewsController wakeUp");
      wakeUp();
   }
}

Core::Scheduling ViewsController::idle() {
   return Core::Scheduling::never();
}

Core::Scheduling ViewsController::active() {
   if(mActiveTimer.expired()) {
      mStateFunction = &ViewsController::idle;
      return Core::Scheduling::never();
   }
   return Core::Scheduling::immediately();
}




} // namespace Ui
} // namespace SolarMonitor
} // namespace Bt
