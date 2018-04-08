//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::Ui::ReadingsView
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_Ui_ReadingsView__h
#define INC__Bt_SolarMonitor_Ui_ReadingsView__h

#include <Bt/Core/I_Runnable.h>
#include "Bt/SolarMonitor/Reader.h"
#include "Bt/SolarMonitor/Ui/Icon.h"
#include "Bt/SolarMonitor/Ui/Display.h"

namespace Bt {
namespace SolarMonitor {
namespace Ui {

class ReadingsView
{
   public:
      ReadingsView(Display& pDisplay);
      ReadingsView(const ReadingsView&) = delete;
      ReadingsView& operator=(const ReadingsView&) = delete;
      ~ReadingsView();

      void update(const Reader::Readings& pReadings);


   private:
      void drawIcon(Icon& pIcon, uint8_t pPosition, bool pActive);
      void drawReading(int16_t pLine ,char pPanel, float pValue, char pUnit);

      Display& mDisplay;
      uint8_t mCounter;
      Reader::Readings mReadings;

};

} // namespace Ui
} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_Ui_ReadingsView__h
