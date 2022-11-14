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

template<typename T>
class AveragingFilter
{
   public:
      typedef std::function<void(const T&)> Consumer;

      AveragingFilter(size_t pCount, const Consumer& pConsumer = Consumer())
      : mCount(pCount), mConsumer(pConsumer){
      }
      AveragingFilter(const AveragingFilter&) = delete;
      AveragingFilter& operator=(const AveragingFilter&) = delete;

      void consume(const T& pValues) {
         mAveraging = mAveraging + pValues;
         if (mAveraging.count() >= mCount) {
            T average = mAveraging.average();
            mAveraging = Averaging<T>();
            if (mConsumer) {
               BT_CORE_LOG_INFO("AveragingFilter: new averaged values");
               mConsumer(average);
            }
         }
      }

      void link(const Consumer& pConsumer) {
         mConsumer = pConsumer;
      }

   private:
      size_t mCount;
      Averaging<T> mAveraging;
      Consumer mConsumer;
      

};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_AveragingFilter__h
