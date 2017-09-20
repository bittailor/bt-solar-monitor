//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::StorageFilter
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_StorageFilter__h
#define INC__Bt_SolarMonitor_StorageFilter__h

#include <array>
#include <functional>
#include "Bt/SolarMonitor/MeasurementRecord.h"

namespace Bt {
namespace SolarMonitor {

template<size_t N>
class StorageFilter
{
   public:
      typedef std::function<void(const std::array<MeasurementRecord, N>&)> Consumer;

      StorageFilter(const Consumer& pConsumer = Consumer()):mIndex(0),mConsumer(pConsumer){

      }

      StorageFilter(const StorageFilter&) = delete;
      StorageFilter& operator=(const StorageFilter&) = delete;

      ~StorageFilter(){

      }

      void consume(const MeasurementRecord& pMeasurementRecord) {
         mStorage[mIndex] = pMeasurementRecord;
         mIndex++;
         if(mIndex >= N) {
            mIndex = 0;
            if(mConsumer) {
               mConsumer(mStorage);
            }
         }
      }

      void link(const Consumer& pConsumer) {
         mConsumer = pConsumer;
      }

   private:
      size_t mIndex;
      std::array<MeasurementRecord, N> mStorage;
      Consumer mConsumer;
      
};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_StorageFilter__h
