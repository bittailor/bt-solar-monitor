//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Sensors::VeDirect::GetCommand
//
//*************************************************************************************************

#ifndef INC__Bt_Sensors_VeDirect_GetCommand__h
#define INC__Bt_Sensors_VeDirect_GetCommand__h

#include <utility>
#include <stdint.h>
#include "Bt/Core/Platform.h"
#include "Bt/Sensors/VeDirect/Register.h"


namespace Bt {
namespace Sensors {
namespace VeDirect {

class GetCommand
{
   public:
      GetCommand(Register pRegister);
      GetCommand(const GetCommand&) = delete;
      GetCommand& operator=(const GetCommand&) = delete;
      ~GetCommand();

      std::pair<bool,int16_t> execute(Stream& pStream);

      const char* cmd() {return mCommandString;}

   private:
      char mCommandString[11];
      

};

} // namespace VeDirect
} // namespace Sensors
} // namespace Bt

#endif // INC__Bt_Sensors_VeDirect_GetCommand__h
