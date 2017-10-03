//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::Platform
//
//*************************************************************************************************

#ifndef INC__Bt_Core_Platform__h
#define INC__Bt_Core_Platform__h

#include "Bt/Core/Platform.h"

#ifdef SPARK_PLATFORM
#include "Particle.h"
#else

#include <string>


//*************************************************************************************************

extern const char* TIME_FORMAT_ISO8601_FULL;

class TimeClass {
   public:
      std::string format(const char* format_spec);
};

extern TimeClass Time;  //eg. usage: Time.day();

//*************************************************************************************************

#endif


namespace Bt {
namespace Core {

} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_Platform__h
