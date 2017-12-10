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
      static const int NUMBER_OF_VALUES = 10;
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

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_Reader__h
