//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::LogFilter
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_LogFilter__h
#define INC__Bt_SolarMonitor_LogFilter__h

#include <inttypes.h>
#include <array>
#include <functional>
#include <Bt/Core/Log.h>
#include <Bt/Core/Result.h>

namespace Bt {
namespace SolarMonitor {


template<size_t N>
class LogFilter
{
   public:
      LogFilter() : mLoopCounter(0), mFailCounter(0){
      }
      LogFilter(const LogFilter&) = delete;
      LogFilter& operator=(const LogFilter&) = delete;
      ~LogFilter() {
      }

      void consume(const std::array<Bt::Core::Result<float>, N>& pReadings) {
         mLoopCounter++;
         BT_CORE_LOG_INFO("loop %" PRIu32 " {%" PRIu32 "}", mLoopCounter, mFailCounter);
         bool allValid = true;
         for(size_t i = 0 ; i < pReadings.size() ; i++) {
            BT_CORE_LOG_INFO(" r[%u]=%5.2f %s", forPrintf(i), pReadings[i].value(), pReadings[i] ? "" : "!");
            allValid = allValid && pReadings[i];
         }
         if(!allValid) {
            mFailCounter++;
         }
      }

   private:
      uint32_t mLoopCounter;
      uint32_t mFailCounter;
};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_LogFilter__h
