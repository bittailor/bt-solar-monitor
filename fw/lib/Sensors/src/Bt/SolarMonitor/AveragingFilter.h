//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::AveragingFilter
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_AveragingFilter__h
#define INC__Bt_SolarMonitor_AveragingFilter__h

#include <functional>
#include "Bt/Core/Log.h"
#include "Bt/SolarMonitor/Averaging.h"
#include "Bt/SolarMonitor/MeasurementRecord.h"

namespace Bt {
namespace SolarMonitor {

template<size_t N>
class AveragingFilter
{
   public:
      typedef std::function<void(const MeasurementRecord<N>&)> Consumer;

      AveragingFilter(size_t pCount, const Consumer& pConsumer = Consumer())
      : mCount(pCount), mConsumer(pConsumer){
      }
      AveragingFilter(const AveragingFilter&) = delete;
      AveragingFilter& operator=(const AveragingFilter&) = delete;

      void consume(const MeasurementRecord<N>& pMeasurementRecord) {
         mAveraging = mAveraging + pMeasurementRecord;
         if (mAveraging.count() >= mCount) {
            MeasurementRecord<N> average = mAveraging.average();
            mAveraging = Averaging<MeasurementRecord<N>>();
            if (mConsumer) {
               BT_CORE_LOG_INFO("AveragingFilter: new averaged reading");
               mConsumer(average);
            }
         }
      }

      void link(const Consumer& pConsumer) {
         mConsumer = pConsumer;
      }

   private:
      size_t mCount;
      Averaging<MeasurementRecord<N>> mAveraging;
      Consumer mConsumer;
      

};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_AveragingFilter__h
