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
#include <Bt/Core/Result.h>
#include <Bt/SolarMonitor/FilterBase.h>


namespace Bt {
namespace SolarMonitor {

template<typename T, size_t N>
class ValidateFilter
{
   public:
      typedef std::function<void(const std::array<T, N>&)> Consumer;

      ValidateFilter(const Consumer& pConsumer): mConsumer(pConsumer) {
      }
      ValidateFilter(const ValidateFilter&) = delete;
      ValidateFilter& operator=(const ValidateFilter&) = delete;

      void consume(const std::array<Bt::Core::Result<T>, N>& pResults) {
         bool allValid = std::accumulate(pResults.begin(), pResults.end(),
                                         true,
                                         [](bool valid, const Bt::Core::Result<T>& result) {
                                            return valid && result;
                                         });

         if(allValid) {
            if (mConsumer) {
               std::array<T, N> values;
               std::transform(pResults.begin(),pResults.end(),
                              values.begin(),
                              [](const Bt::Core::Result<T>& r) -> T {return r.value();});
               mConsumer(values);
            }
         } else {
            BT_CORE_LOG_WARN("ValidateFilter: At least one result was invalid");
         }
      }

   private:
      Consumer mConsumer;
};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_ValidateFilter__h
