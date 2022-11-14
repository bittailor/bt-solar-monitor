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

   float seedEmulated() {
      float r = random(10,150);
      return r / 100;
   }

} // namespace


EmulatedReader::EmulatedReader(Stream& pStreamA, Stream& pStreamB) {

}

EmulatedReader::~EmulatedReader() {

}

EmulatedReader::Readings EmulatedReader::read() {
   EmulatedReader::Readings readings;
      readings[checkedIndex<0>()] = 3 + seedEmulated();
      readings[checkedIndex<1>()] = 13 + seedEmulated();
      readings[checkedIndex<2>()] = 0.5 + seedEmulated();
      readings[checkedIndex<3>()] = 12 + seedEmulated();
      readings[checkedIndex<4>()] = 0.2 + seedEmulated();

      readings[checkedIndex<5>()] = 3 + seedEmulated();
      readings[checkedIndex<6>()] = 13 + seedEmulated();
      readings[checkedIndex<7>()] = 0.5 + seedEmulated();
      readings[checkedIndex<8>()] = 12 + seedEmulated();
      readings[checkedIndex<9>()] = 0.2 + seedEmulated();

   return readings;
}

} // namespace SolarMonitor
} // namespace Bt
