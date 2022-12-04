//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::Reader
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_Reader__h
#define INC__Bt_SolarMonitor_Reader__h

#include <array>
#include <Bt/Sensors/VeDirect/Register.h>

namespace Bt {
namespace SolarMonitor {

class Reader
{
   public:
      static constexpr size_t NUMBER_OF_VALUES = 10;
      typedef std::array<Core::Result<float>,NUMBER_OF_VALUES> Readings;

      Reader(Stream& pStreamA, Stream& pStreamB);
      Reader(const Reader&) = delete;
      Reader& operator=(const Reader&) = delete;
      ~Reader();

      Readings read();

   private:

      Sensors::VeDirect::Register<uint32_t> mPanelPower;
      Sensors::VeDirect::Register<uint16_t> mPanelVoltage;
      Sensors::VeDirect::Register<uint16_t> mChargerCurrent;
      Sensors::VeDirect::Register<uint16_t> mChargerVoltage;
      Sensors::VeDirect::Register<uint16_t> mLoadCurrent;
      Stream& mStreamA;
      Stream& mStreamB;     
};

class SolarReading {
   
   public:
      enum Facility {A=0,B=1};

      static SolarReading unpack(const Reader::Readings& pReadings, Facility pFacility ) {
         return SolarReading(pReadings,pFacility);     
      }

      Bt::Core::Result<float> panelPower() const {
         return mReadings[(mFacility*READINGS_PER_FACILITY)];
      }
      Bt::Core::Result<float> panelVoltage() const {
         return mReadings[(mFacility*READINGS_PER_FACILITY+1)];
      }
      Bt::Core::Result<float> chargerCurrent() const {
         return mReadings[(mFacility*READINGS_PER_FACILITY+2)];
      } 
      Bt::Core::Result<float> chargerVoltage() const{
         return mReadings[(mFacility*READINGS_PER_FACILITY+3)];
      } 
      Bt::Core::Result<float> loadCurrent() const{
         return mReadings[(mFacility*READINGS_PER_FACILITY+4)];
      } 
   
   private:
      static constexpr size_t READINGS_PER_FACILITY = 5;

      SolarReading(const Reader::Readings& pReadings, Facility pFacility):mReadings(pReadings),mFacility(pFacility){}
      const Reader::Readings& mReadings; 
      Facility mFacility;
};


} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_Reader__h
