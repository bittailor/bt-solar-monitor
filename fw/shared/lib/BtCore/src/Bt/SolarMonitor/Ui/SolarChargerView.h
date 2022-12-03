//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::Ui::SolarChargerView
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_Ui_SolarChargerView__h
#define INC__Bt_SolarMonitor_Ui_SolarChargerView__h

#include "Bt/Ui/I_Canvas.h"
#include "Bt/SolarMonitor/Reader.h"

namespace Bt {
namespace SolarMonitor {
namespace Ui {

class SolarChargerView
{
   public:
      SolarChargerView();
      SolarChargerView(const SolarChargerView&) = delete;
      SolarChargerView& operator=(const SolarChargerView&) = delete;
      ~SolarChargerView();

      void render(::Bt::Ui::I_Canvas& pCanvas);

      void update(Bt::SolarMonitor::Reader::Readings& pReadings);

   private:
      Bt::SolarMonitor::Reader::Readings mReadings;
      
};

} // namespace Ui
} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_Ui_SolarChargerView__h

