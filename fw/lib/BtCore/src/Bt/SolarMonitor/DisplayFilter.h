//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::DisplayFilter
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_DisplayFilter__h
#define INC__Bt_SolarMonitor_DisplayFilter__h

#include <array>
#include <functional>
#include "Adafruit_GFX.h"
#include "Adafruit_PCD8544.h"
#include <Bt/Core/Log.h>
#include <Bt/Core/Result.h>
#include <Bt/Sensors/INA219.h>

namespace Bt {
namespace SolarMonitor {


template<size_t N>
class DisplayFilter
{
   public:
      DisplayFilter(Adafruit_PCD8544& pDisplay) : mLoopCounter(0), mFailCounter(0), mDisplay(pDisplay) {

      }
      DisplayFilter(const DisplayFilter&) = delete;
      DisplayFilter& operator=(const DisplayFilter&) = delete;
      ~DisplayFilter() {
      }

      void consume(const std::array<Bt::Core::Result<float>, N>& pReadings) {
         const size_t BUFFER_SIZE = 50;
         char buffer[BUFFER_SIZE] = {0};
         mLoopCounter++;
         mDisplay.clearDisplay();
         mDisplay.setTextSize(1);
         snprintf(buffer, BUFFER_SIZE, "Loop %lu", mLoopCounter);
         mDisplay.setCursor(0,0);
         mDisplay.print(buffer);
         for(size_t i = 0 ; i < pReadings.size() ; i++) {
            buffer[0] = 0;
            if(pReadings[i]) {
               snprintf(buffer, BUFFER_SIZE, "%.2f", pReadings[i].value());
            } else {
               snprintf(buffer, BUFFER_SIZE, "--!--");
            }
            mDisplay.setCursor((i%2)*42, ((i/2)+1)*8);
            mDisplay.print(buffer);
            BT_CORE_LOG_DEBUG("value - [%s] v = %f", pReadings[i] ? "valid" : "invalid"  ,pReadings[i].value());
         }
         mDisplay.display();
      }

   private:
      uint32_t mLoopCounter;
      uint32_t mFailCounter;
      Adafruit_PCD8544& mDisplay;

      
};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_DisplayFilter__h
