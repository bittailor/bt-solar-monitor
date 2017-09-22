//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::FilterBase
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_FilterBase__h
#define INC__Bt_SolarMonitor_FilterBase__h

#include <array>
#include <functional>

namespace Bt {
namespace SolarMonitor {

template<typename T>
class FilterBase
{
   public:
      typedef FilterBase<T> Base;
      typedef std::function<void(const T&)> Consumer;

      FilterBase(const Consumer& pConsumer):mConsumer(pConsumer){
      }

      void link(const Consumer& pConsumer) {
         mConsumer = pConsumer;
      }

   protected:
      Consumer mConsumer;
};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_FilterBase__h
