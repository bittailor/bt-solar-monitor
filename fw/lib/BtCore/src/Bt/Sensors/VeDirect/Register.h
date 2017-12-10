//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Sensors::VeDirect::Register
//
//*************************************************************************************************

#ifndef INC__Bt_Sensors_VeDirect_Register__h
#define INC__Bt_Sensors_VeDirect_Register__h

#include <stdint.h>

#include <Bt/Core/Platform.h>

#include <Bt/Core/Result.h>

#include "Bt/Sensors/VeDirect/GetCommand.h"

namespace Bt {
namespace Sensors {
namespace VeDirect {

template<typename T>
class Register {
   public:
      Register(uint16_t pRegister, uint16_t pScale):mGetCommand(pRegister), mScale(pScale){
      }

      Core::Result<T> get(Stream& pStream) {
         return mGetCommand.execute(pStream);
      }

      Core::Result<float> scaled(Stream& pStream) {
         Core::Result<T> raw = get(pStream);
         if(raw) {
            float value = raw.value();
            return value / mScale;
         }
         return Core::Result<float>();
      }

   private:
      GetCommand<T> mGetCommand;
      uint16_t mScale;

};


} // namespace VeDirect
} // namespace Sensors
} // namespace Bt

#endif // INC__Bt_Sensors_VeDirect_Register__h
