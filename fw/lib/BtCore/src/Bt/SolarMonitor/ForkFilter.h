//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::ForkFilter
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_ForkFilter__h
#define INC__Bt_SolarMonitor_ForkFilter__h

#include <array>
#include <functional>

namespace Bt {
namespace SolarMonitor {

template<typename T, size_t N>
class ForkFilter
{
   public:
      typedef std::function<void(const T&)> Consumer;
      typedef std::array<Consumer,N> Consumers;

      ForkFilter(const std::array<Consumer,N>& pConsumers = std::array<Consumer,N>())
      : mConsumers(pConsumers){

      }
      ForkFilter(const ForkFilter&) = delete;
      ForkFilter& operator=(const ForkFilter&) = delete;
      ~ForkFilter() {
      }

      void consume(const T& pValue) {
         for(Consumer& consumer : mConsumers) {
            consumer(pValue);
         }
      }

      void link(const Consumer& pConsumer, size_t pIndex) {
         mConsumers[pIndex] = pConsumer;
      }

   private:
      std::array<Consumer,N> mConsumers;
      
};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_ForkFilter__h
