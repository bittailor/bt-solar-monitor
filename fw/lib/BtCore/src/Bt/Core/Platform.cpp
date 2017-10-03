//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::Platform
//
//*************************************************************************************************

#include "Bt/Core/Platform.h"


#ifdef SPARK_PLATFORM
#include "Particle.h"
#else


//*************************************************************************************************

const char* TIME_FORMAT_ISO8601_FULL = "%Y-%m-%dT%H:%M:%S%z";

std::string TimeClass::format(const char* format_spec) {
   return "2017-10-01T13:14:15Z";
}

TimeClass Time;

//*************************************************************************************************

#endif

namespace Bt {
namespace Core {

} // namespace Core
} // namespace Bt
