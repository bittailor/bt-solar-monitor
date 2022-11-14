//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::Reading
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_Reading__h
#define INC__Bt_SolarMonitor_Reading__h

namespace Bt {
namespace SolarMonitor {

class Reading
{
   public:
      Reading() : mCurrent(0), mVoltage(0) {
      }

      Reading(float pCurrent, float pVoltage) : mCurrent(pCurrent), mVoltage(pVoltage) {
      }

      float current() const { return mCurrent; }
      float voltage() const { return mVoltage; }

      friend const Reading operator+(const Reading& lhs, const Reading& rhs) {
         return Reading{lhs.mCurrent + rhs.mCurrent, lhs.mVoltage + rhs.mVoltage};
      }

      friend const Reading operator/(const Reading& lhs, int rhs) {
         return Reading{lhs.mCurrent / rhs, lhs.mVoltage / rhs};
      }

   private:

      float mCurrent;
      float mVoltage;
};




} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_Reading__h
