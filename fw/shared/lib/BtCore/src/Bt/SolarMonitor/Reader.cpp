//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::Reader
//
//*************************************************************************************************

#include "Bt/SolarMonitor/Reader.h"

#include <Bt/Sensors/VeDirect/Registers.h>

namespace Bt {
namespace SolarMonitor {
namespace {

   template<size_t I>
   size_t checkedIndex(){
      static_assert(I < Reader::NUMBER_OF_VALUES, "Index Out Of Range");
      return I;
   }

} // namespace

Reader::Reader(Stream& pStreamA, Stream& pStreamB)
: mPanelPower(Sensors::VeDirect::Registers::panelPower())
, mPanelVoltage(Sensors::VeDirect::Registers::panelVoltage())
, mChargerCurrent(Sensors::VeDirect::Registers::chargerCurrent())
, mChargerVoltage(Sensors::VeDirect::Registers::chargerVoltage())
, mLoadCurrent(Sensors::VeDirect::Registers::loadCurrent())
, mStreamA(pStreamA)
, mStreamB(pStreamB){

}

Reader::~Reader() {

}


Reader::Readings Reader::read() {
   Reader::Readings readings;
   readings[checkedIndex<0>()] = mPanelPower.scaled(mStreamA);
   readings[checkedIndex<1>()] = mPanelVoltage.scaled(mStreamA);
   readings[checkedIndex<2>()] = mChargerCurrent.scaled(mStreamA);
   readings[checkedIndex<3>()] = mChargerVoltage.scaled(mStreamA);
   readings[checkedIndex<4>()] = mLoadCurrent.scaled(mStreamA);

   readings[checkedIndex<5>()] = mPanelPower.scaled(mStreamB);
   readings[checkedIndex<6>()] = mPanelVoltage.scaled(mStreamB);
   readings[checkedIndex<7>()] = mChargerCurrent.scaled(mStreamB);
   readings[checkedIndex<8>()] = mChargerVoltage.scaled(mStreamB);
   readings[checkedIndex<9>()] = mLoadCurrent.scaled(mStreamB);

   return readings;
}



 



} // namespace SolarMonitor
} // namespace Bt
