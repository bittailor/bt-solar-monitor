//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::Scheduling
//
//*************************************************************************************************

#include "Bt/Core/Scheduling.h"

namespace Bt {
namespace Core {

const char* Scheduling::typeString() {
   switch(mType) {
      case IMMEDIATELY : return "IMMEDIATELY";
      case MILLIS_DELAY : return "MILLIS_DELAY";
      case SECONDS_DELAY : return "SECONDS_DELAY";
      case NEVER : return "NEVER";
   }
   return "!";
}

} // namespace Core
} // namespace Bt
