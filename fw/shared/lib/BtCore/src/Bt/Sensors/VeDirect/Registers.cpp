//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Sensors::VeDirect::Registers
//
//*************************************************************************************************

#include "Bt/Sensors/VeDirect/Registers.h"

namespace Bt {
namespace Sensors {
namespace VeDirect {


Core::Result<String> Version::read(Stream& pStream) {
   CommandExecutor executor(pStream,":154");
   const char* rsp = executor.execute();
   if(strlen(rsp) != 8) {
      return Core::Result<String>();
   }
   String version;
   version.reserve(6);
   version += rsp[5];
   version += '.';
   version += rsp[2];
   version += rsp[3];
   return Core::Result<String>(version);
}

} // namespace VeDirect
} // namespace Sensors
} // namespace Bt
