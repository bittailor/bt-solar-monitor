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
#include <Bt/SolarMonitor/Ui/ReadingsView.h>

namespace Bt {
namespace SolarMonitor {


template<size_t N>
class DisplayFilter
{
   public:
      DisplayFilter(Ui::ReadingsView& pView) : mLoopCounter(0), mFailCounter(0), mView(pView) {
      }
      DisplayFilter(const DisplayFilter&) = delete;
      DisplayFilter& operator=(const DisplayFilter&) = delete;
      ~DisplayFilter() {
      }

      void consume(const std::array<Bt::Core::Result<float>, N>& pReadings) {

         mView.update(pReadings);

         const size_t BUFFER_SIZE = 50;
         char buffer[BUFFER_SIZE] = {0};
         mLoopCounter++;
         snprintf(buffer, BUFFER_SIZE, "Loop %lu", mLoopCounter);
         for(size_t i = 0 ; i < pReadings.size() ; i++) {
            BT_CORE_LOG_DEBUG("value - [%s] v = %f", pReadings[i] ? "valid" : "invalid"  ,pReadings[i].value());
         }
      }

   private:
      uint32_t mLoopCounter;
      uint32_t mFailCounter;
      Ui::ReadingsView& mView;

      
};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_DisplayFilter__h
