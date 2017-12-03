//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Sensors::VeDirect::Registers
//
//*************************************************************************************************

#ifndef INC__Bt_Sensors_VeDirect_Registers__h
#define INC__Bt_Sensors_VeDirect_Registers__h

#include "Bt/Sensors/VeDirect/Register.h"

namespace Bt {
namespace Sensors {
namespace VeDirect {

class Registers
{
   public:
      static Register<uint16_t> chargerCurrent() {return Register<uint16_t>(0xEDD7);}
      static Register<uint16_t> chargerVoltage() {return Register<uint16_t>(0xEDD5);}

      static Register<uint32_t> panelPower() {return Register<uint32_t>(0xEDBC);}
      static Register<uint16_t> panelVoltage() {return Register<uint16_t>(0xEDBB);}

      static Register<uint16_t> loadCurrent() {return Register<uint16_t>(0xEDAD);}
      static Register<bool> loadOutputState() {return Register<bool>(0xEDA8);}

      static Register<uint16_t> batteryMaximumCurrent() {return Register<uint16_t>(0xEDF0);}

};

} // namespace VeDirect
} // namespace Sensors
} // namespace Bt

#endif // INC__Bt_Sensors_VeDirect_Registers__h
