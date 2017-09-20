//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::Measurement
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_Measurement__h
#define INC__Bt_SolarMonitor_Measurement__h

namespace Bt {
namespace SolarMonitor {

class Measurement
{
   public:
      Measurement() : mCurrent(0), mVoltage(0) {
      }

      Measurement(float pCurrent, float pVoltage) : mCurrent(pCurrent), mVoltage(pVoltage) {
      }

      float current() const { return mCurrent; }
      float voltage() const { return mVoltage; }

      friend const Measurement operator+(const Measurement& lhs, const Measurement& rhs) {
         return Measurement{lhs.mCurrent + rhs.mCurrent, lhs.mVoltage + rhs.mVoltage};
      }

      friend const Measurement operator/(const Measurement& lhs, int rhs) {
         return Measurement{lhs.mCurrent / rhs, lhs.mVoltage / rhs};
      }

   private:

      float mCurrent;
      float mVoltage;
};




} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_Measurement__h
