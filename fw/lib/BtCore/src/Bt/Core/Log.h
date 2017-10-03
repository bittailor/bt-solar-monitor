//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::Log
//
//*************************************************************************************************

#ifndef INC__Bt_Core_Log__hpp
#define INC__Bt_Core_Log__hpp

#include <Bt/Core/Platform.h>


#ifdef SPARK_PLATFORM
#define BT_CORE_LOG_DEBUG(formatString, ...) Log.trace(formatString,##__VA_ARGS__)
//#define BT_CORE_LOG_DEBUG(formatString, ...)

#define BT_CORE_LOG_INFO(formatString, ...)  Log.info(formatString,##__VA_ARGS__)

#define BT_CORE_LOG_WARN(formatString, ...)  Log.warn(formatString,##__VA_ARGS__)

#define BT_CORE_LOG_ERROR(formatString, ...) Log.error(formatString,##__VA_ARGS__)
#else
#define BT_CORE_LOG_DEBUG(formatString, ...) printf("DEBUG: " formatString "\n", ##__VA_ARGS__)

#define BT_CORE_LOG_INFO(formatString, ...)  printf("INFO: " formatString "\n", ##__VA_ARGS__)

#define BT_CORE_LOG_WARN(formatString, ...)  printf("WARN: " formatString "\n", ##__VA_ARGS__)

#define BT_CORE_LOG_ERROR(formatString, ...) printf("ERROR: " formatString "\n", ##__VA_ARGS__)
#endif


namespace Bt {
namespace Core {

} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_Log__hpp
