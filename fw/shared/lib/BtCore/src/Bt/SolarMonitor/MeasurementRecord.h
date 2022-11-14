//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::MeasurementRecord
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_MeasurementRecord__h
#define INC__Bt_SolarMonitor_MeasurementRecord__h

#include <array>
#include <algorithm>
#include <Bt/SolarMonitor/Reading.h>

namespace Bt {
namespace SolarMonitor {

template <size_t N>
using MeasurementRecord = std::array<Reading,N>;

template <size_t N>
inline const MeasurementRecord<N> operator+(const MeasurementRecord<N> lhs,const MeasurementRecord<N>& rhs)
{
  MeasurementRecord<N> sum;
  std::transform(
           lhs.begin(),lhs.end(),
           rhs.begin(),
           sum.begin(),
           [](const Reading& l, const Reading& r) -> Reading {return l+r;}
  );
  return sum;
}

template <size_t N>
inline const MeasurementRecord<N> operator/(const MeasurementRecord<N> lhs, double rhs)
{
   MeasurementRecord<N> div;
   std::transform(
            lhs.begin(),lhs.end(),
            div.begin(),
            [rhs](const Reading& l) -> Reading {return l/rhs;}
   );
   return div;
}

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_MeasurementRecord__h
