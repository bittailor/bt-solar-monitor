//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::DisplayFilter
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_DisplayFilter__h
#define INC__Bt_SolarMonitor_DisplayFilter__h

#include <array>
#include <functional>
#include <nokia-5110-lcd.h>
#include <Bt/Core/Log.h>
#include <Bt/Sensors/INA219.h>

namespace Bt {
namespace SolarMonitor {


template<size_t N>
class DisplayFilter
{
   public:
      DisplayFilter(Nokia5110LCD::Display& pDisplay) : mLoopCounter(0), mFailCounter(0), mDisplay(pDisplay) {

      }
      DisplayFilter(const DisplayFilter&) = delete;
      DisplayFilter& operator=(const DisplayFilter&) = delete;
      ~DisplayFilter() {
      }

      void consume(const std::array<Bt::Sensors::INA219Reading, N>& pReadings) {
         const size_t BUFFER_SIZE = 50;
         char buffer[BUFFER_SIZE] = {0};
         mLoopCounter++;
         mDisplay.clearDisplay();
         snprintf(buffer, BUFFER_SIZE, "Loop %lu", mLoopCounter);
         mDisplay.setStr(buffer, 0, 0, BLACK);
         for(std::size_t i = 0 ; i < pReadings.size() ; i++) {
            const Bt::Sensors::INA219Reading& reading = pReadings[i];
            buffer[0] = 0;
            if(reading.valid) {
               snprintf(buffer, BUFFER_SIZE, "%.2f %.3f", reading.busVoltage, reading.current);
            } else {
               snprintf(buffer, BUFFER_SIZE, "--!--");
            }
            mDisplay.setStr(buffer, 0, (i+1)*8, BLACK);
            BT_CORE_LOG_DEBUG("Reading - [%s] U = %f I = %f", reading.valid ? "valid" : "invalid"  ,reading.busVoltage, reading.current);
         }
         mDisplay.updateDisplay();
      }

   private:
      uint32_t mLoopCounter;
      uint32_t mFailCounter;
      Nokia5110LCD::Display& mDisplay;

      
};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_DisplayFilter__h
