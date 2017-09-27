//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::ValidateFilter
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_ValidateFilter__h
#define INC__Bt_SolarMonitor_ValidateFilter__h

#include <array>
#include <functional>
#include <numeric>
#include <algorithm>

#include <Bt/Core/Log.h>
#include <Bt/Sensors/INA219.h>
#include <Bt/SolarMonitor/FilterBase.h>
#include <Bt/SolarMonitor/Reading.h>


namespace Bt {
namespace SolarMonitor {

template<size_t N>
class ValidateFilter
{
   public:
      typedef std::function<void(const std::array<Bt::SolarMonitor::Reading, N>&)> Consumer;

      ValidateFilter(const Consumer& pConsumer): mConsumer(pConsumer) {
      }
      ValidateFilter(const ValidateFilter&) = delete;
      ValidateFilter& operator=(const ValidateFilter&) = delete;

      void consume(const std::array<Bt::Sensors::INA219Reading, N>& pReadings) {
         bool allValid = std::accumulate(pReadings.begin(), pReadings.end(),
                                         true,
                                         [](bool valid,const Bt::Sensors::INA219Reading& reading) {
                                            return valid && reading.valid;
                                         });

         if(allValid) {
            if (mConsumer) {
               std::array<Bt::SolarMonitor::Reading, N> measurements;
               std::transform(pReadings.begin(),pReadings.end(),
                              measurements.begin(),
                              [](const Sensors::INA219Reading& reading) -> Reading {
                                 return Reading{reading.current, reading.busVoltage};
                              });
               mConsumer(measurements);
            }
         } else {
            BT_CORE_LOG_WARN("ValidateFilter: At least one INA219Reading was invalid");
         }
      }

   private:
      Consumer mConsumer;
};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_ValidateFilter__h
