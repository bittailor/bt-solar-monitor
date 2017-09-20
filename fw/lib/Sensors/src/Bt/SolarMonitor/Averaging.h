//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::Averaging
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_Averaging__h
#define INC__Bt_SolarMonitor_Averaging__h

#include <stddef.h>

namespace Bt {
namespace SolarMonitor {


template<typename T>
class Averaging
{
   public:
      Averaging():mCounter(0),mValue(){
      }
      ~Averaging() {
      }

      size_t count() {
         return mCounter;
      }

      T average() {
         if (mCounter == 0) {
            return mValue;
         }
         return mValue / mCounter;
      }

      friend const Averaging operator+(const Averaging& lhs, const T& rhs) {
         return Averaging{lhs.mCounter + 1, lhs.mValue + rhs};
      }

   private:
      Averaging(size_t pCounter, T pValue):mCounter(pCounter),mValue(pValue){
      }

      size_t mCounter;
      T mValue;

};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_Averaging__h
