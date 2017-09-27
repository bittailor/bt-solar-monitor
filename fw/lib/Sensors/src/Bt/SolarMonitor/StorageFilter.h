//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::StorageFilter
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_StorageFilter__h
#define INC__Bt_SolarMonitor_StorageFilter__h

#include <array>
#include <functional>
#include "Bt/Core/Log.h"
#include "Bt/SolarMonitor/MeasurementRecord.h"

namespace Bt {
namespace SolarMonitor {

template<size_t N, size_t R>
class StorageFilter
{
   public:
      typedef std::function<void(const std::array<MeasurementRecord<R>, N>&)> Consumer;

      StorageFilter(const Consumer& pConsumer = Consumer()):mIndex(0),mConsumer(pConsumer){

      }

      StorageFilter(const StorageFilter&) = delete;
      StorageFilter& operator=(const StorageFilter&) = delete;

      ~StorageFilter(){

      }

      void consume(const MeasurementRecord<R>& pMeasurementRecord) {
         mStorage[mIndex] = pMeasurementRecord;
         mIndex++;
         if(mIndex >= N) {
            mIndex = 0;
            if(mConsumer) {
               BT_CORE_LOG_INFO("StorageFilter: storage full send it out.");
               mConsumer(mStorage);
            }
         }
      }

      void link(const Consumer& pConsumer) {
         mConsumer = pConsumer;
      }

   private:
      size_t mIndex;
      std::array<MeasurementRecord<R>, N> mStorage;
      Consumer mConsumer;
      
};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_StorageFilter__h
