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
#include <Bt/Core/Result.h>
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

      void consume(const std::array<Bt::Core::Result<float>, N>& pReadings) {
         const size_t BUFFER_SIZE = 50;
         char buffer[BUFFER_SIZE] = {0};
         mLoopCounter++;
         mDisplay.clearDisplay();
         snprintf(buffer, BUFFER_SIZE, "Loop %lu", mLoopCounter);
         mDisplay.setStr(buffer, 0, 0, BLACK);
         for(size_t i = 0 ; i < pReadings.size() ; i++) {
            buffer[0] = 0;
            if(pReadings[i]) {
               snprintf(buffer, BUFFER_SIZE, "%.2f", pReadings[i].value());
            } else {
               snprintf(buffer, BUFFER_SIZE, "--!--");
            }
            mDisplay.setStr(buffer, (i%2)*42, ((i/2)+1)*8, BLACK);
            BT_CORE_LOG_DEBUG("value - [%s] v = %f", pReadings[i] ? "valid" : "invalid"  ,pReadings[i].value());
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
