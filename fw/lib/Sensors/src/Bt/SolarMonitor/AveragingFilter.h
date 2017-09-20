//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::AveragingFilter
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_AveragingFilter__h
#define INC__Bt_SolarMonitor_AveragingFilter__h

#include <functional>
#include "Bt/SolarMonitor/Averaging.h"
#include "Bt/SolarMonitor/MeasurementRecord.h"

namespace Bt {
namespace SolarMonitor {

class AveragingFilter
{
   public:

      typedef std::function<void(const MeasurementRecord&)> Consumer;

      AveragingFilter(size_t pCount, const Consumer& pConsumer = Consumer());
      AveragingFilter(const AveragingFilter&) = delete;
      AveragingFilter& operator=(const AveragingFilter&) = delete;
      ~AveragingFilter();

      void consume(const MeasurementRecord& pMeasurementRecord);
      void link(const Consumer& pConsumer);

   private:
      size_t mCount;
      Averaging<MeasurementRecord> mAveraging;
      Consumer mConsumer;
      

};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_AveragingFilter__h
