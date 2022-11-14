//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::Platform
//
//*************************************************************************************************

#ifndef INC__Bt_Core_Platform__h
#define INC__Bt_Core_Platform__h

#ifdef SPARK_PLATFORM

#include <Particle.h>

inline size_t forPrintf(size_t s) { return s;}

#else

#include <PlatformMock.h>

inline unsigned forPrintf(size_t s) { return (unsigned)s;}

#endif


#endif // INC__Bt_Core_Platform__h
