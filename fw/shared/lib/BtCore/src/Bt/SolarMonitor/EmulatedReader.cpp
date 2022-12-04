//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::EmulatedReader
//
//*************************************************************************************************

#include "Bt/SolarMonitor/EmulatedReader.h"

namespace Bt {
namespace SolarMonitor {
namespace {

   template<size_t I>
   size_t checkedIndex(){
      static_assert(I < EmulatedReader::NUMBER_OF_VALUES, "Index Out Of Range");
      return I;
   }

   float seedEmulated(float pMin = 0.1, float pMax = 1.5) {
      float r = random(pMin*100, pMax*100);
      return r / 100;
   }

} // namespace


EmulatedReader::EmulatedReader(Stream& pStreamA, Stream& pStreamB) {

}

EmulatedReader::~EmulatedReader() {

}

EmulatedReader::Readings EmulatedReader::read() {
   EmulatedReader::Readings readings;
      readings[checkedIndex<0>()] = seedEmulated(0,80);
      readings[checkedIndex<1>()] = seedEmulated(0,20);
      readings[checkedIndex<2>()] = seedEmulated(0.1,9);
      readings[checkedIndex<3>()] = seedEmulated(10,15);
      readings[checkedIndex<4>()] = seedEmulated(0,9);

      readings[checkedIndex<5>()] = seedEmulated(0,80);
      readings[checkedIndex<6>()] = seedEmulated(0,20);
      readings[checkedIndex<7>()] = seedEmulated(0.1,9);
      readings[checkedIndex<8>()] = seedEmulated(10,15);
      readings[checkedIndex<9>()] = seedEmulated(0,9);

   return readings;
}

} // namespace SolarMonitor
} // namespace Bt
