//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::Averaging
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_Averaging__h
#define INC__Bt_SolarMonitor_Averaging__h

#include <array>
#include <algorithm>
#include <stddef.h>

namespace Bt {
namespace SolarMonitor {

template<typename T, size_t N>
inline const std::array<T,N> operator+(const std::array<T,N>& lhs,const std::array<T,N>& rhs)
{
  std::array<T,N> sum;
  std::transform(
           lhs.begin(),lhs.end(),
           rhs.begin(),
           sum.begin(),
           [](const T& l, const T& r) -> T {return l+r;}
  );
  return sum;
}

template<typename T, size_t N>
inline const std::array<T,N> operator/(const std::array<T,N>& lhs, double rhs)
{
   std::array<T,N> div;
   std::transform(
            lhs.begin(),lhs.end(),
            div.begin(),
            [rhs](const T& l) -> T {return l/rhs;}
   );
   return div;
}


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
