//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::Log
//
//*************************************************************************************************

#ifndef INC__Bt_Core_Log__hpp
#define INC__Bt_Core_Log__hpp

// #define BT_CORE_LOG_DEBUG(formatString, ...) Log.trace(String::format(formatString,##__VA_ARGS__))
#define BT_CORE_LOG_DEBUG(formatString, ...)

#define BT_CORE_LOG_INFO(formatString, ...)  Log.info(String::format(formatString,##__VA_ARGS__))

#define BT_CORE_LOG_WARN(formatString, ...)  Log.warn(String::format(formatString,##__VA_ARGS__))

#define BT_CORE_LOG_ERROR(formatString, ...) Log.error(String::format(formatString,##__VA_ARGS__))

namespace Bt {
namespace Core {

} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_Log__hpp
