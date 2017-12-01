//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Sensors::VeDirect::Register
//
//*************************************************************************************************

#ifndef INC__Bt_Sensors_VeDirect_Register__h
#define INC__Bt_Sensors_VeDirect_Register__h

#include <stdint.h>

namespace Bt {
namespace Sensors {
namespace VeDirect {

enum class Register : uint16_t {
   ChargerCurrent = 0xEDD7,
   ChargerVoltage = 0xEDD5,

   PanelPower = 0xEDBC,
   PanelVoltage = 0xEDBB,

   LoadCurrent = 0xEDAD,
   LoadOutputState = 0xEDA8,

   BatteryMaximumCurrent = 0xEDF0,

};

} // namespace VeDirect
} // namespace Sensors
} // namespace Bt

#endif // INC__Bt_Sensors_VeDirect_Register__h
