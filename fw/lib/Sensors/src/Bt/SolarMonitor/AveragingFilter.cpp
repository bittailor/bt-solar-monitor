//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::AveragingFilter
//
//*************************************************************************************************

#include "Bt/SolarMonitor/AveragingFilter.h"

namespace Bt {
namespace SolarMonitor {

AveragingFilter::AveragingFilter(size_t pCount, const Consumer& pConsumer)
: mCount(pCount), mConsumer(pConsumer){

}

AveragingFilter::~AveragingFilter() {

}

void AveragingFilter::consume(const MeasurementRecord& pMeasurementRecord) {
   mAveraging = mAveraging + pMeasurementRecord;
   if (mAveraging.count() >= mCount) {
      MeasurementRecord average = mAveraging.average();
      mAveraging = Averaging<MeasurementRecord>();
      if (mConsumer) {
         mConsumer(average);
      }
   }
}

void AveragingFilter::link(const Consumer& pConsumer) {
   mConsumer = pConsumer;
}

} // namespace SolarMonitor
} // namespace Bt
