//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::MeasurementRecord
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_MeasurementRecord__h
#define INC__Bt_SolarMonitor_MeasurementRecord__h

#include <array>
#include <algorithm>
#include <Bt/Sensors/INA219.h>
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

/*
class MeasurementRecord
{
   public:
      static const size_t NUMBER_OF_MEASUREMENTS = 6;

      MeasurementRecord() = default;

      MeasurementRecord(const std::array<Sensors::INA219Reading,NUMBER_OF_MEASUREMENTS>& pReadings) {
         std::transform(pReadings.begin(),pReadings.end(),
                        mMeasurements.begin(),
                        [](const Sensors::INA219Reading& reading) -> Measurement { return Measurement{reading.current, reading.busVoltage};});
      }

      MeasurementRecord(const std::array<Measurement,NUMBER_OF_MEASUREMENTS>& pMeasurements)
         :mMeasurements(pMeasurements) {
      }

      const Measurement& panelA() const {return mMeasurements[0];}
      const Measurement& panelB() const {return mMeasurements[1];}
      const Measurement& batteryA() const {return mMeasurements[2];}
      const Measurement& batteryB() const {return mMeasurements[3];}
      const Measurement& load() const {return mMeasurements[4];}
      const Measurement& control() const {return mMeasurements[5];}

      friend const MeasurementRecord operator+(const MeasurementRecord lhs,const MeasurementRecord& rhs)
      {
         std::array<Measurement,NUMBER_OF_MEASUREMENTS> sum;
         std::transform(
                  lhs.mMeasurements.begin(),lhs.mMeasurements.end(),
                  rhs.mMeasurements.begin(),
                  sum.begin(),
                  [](const Measurement& l, const Measurement& r) -> Measurement {return l+r;}
         );
         return sum;
      }

      friend const MeasurementRecord operator/(const MeasurementRecord lhs, double rhs)
      {
         std::array<Measurement,NUMBER_OF_MEASUREMENTS> div;
         std::transform(
                  lhs.mMeasurements.begin(),lhs.mMeasurements.end(),
                  div.begin(),
                  [rhs](const Measurement& l) -> Measurement {return l/rhs;}
         );
         return div;
      }

   private:
      std::array<Measurement,NUMBER_OF_MEASUREMENTS> mMeasurements;
};
*/

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_MeasurementRecord__h
