//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::Platform
//
//*************************************************************************************************

#ifndef INC__Bt_Core_Platform__h
#define INC__Bt_Core_Platform__h

#ifdef SPARK_PLATFORM
#include <Particle.h>
#else
#include <PlatformMock.h>
#endif


namespace Bt {
namespace Core {

} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_Platform__h
