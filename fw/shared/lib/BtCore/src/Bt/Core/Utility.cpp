//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::Utility
//
//*************************************************************************************************

#include "Bt/Core/Utility.h"
#include <cmath>

namespace Bt {
namespace Core {

std::tuple<int,int> split(float pValue, int pDecimals) {
   auto factor = std::pow(10,pDecimals);
   auto value = std::round( factor * pValue);
   int a = value / factor;
   int b = value - (a * factor);
   return {a,b};
}

} // namespace Core
} // namespace Bt
