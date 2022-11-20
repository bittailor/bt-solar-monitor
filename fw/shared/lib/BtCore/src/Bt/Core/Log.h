//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::Log
//
//*************************************************************************************************

#ifndef INC__Bt_Core_Log__h
#define INC__Bt_Core_Log__h

#include <Bt/Core/Platform.h>

#define BT_CORE_LOG_DEBUG(formatString, ...) Log.trace(formatString,##__VA_ARGS__)
//#define BT_CORE_LOG_DEBUG(formatString, ...)

#define BT_CORE_LOG_INFO(formatString, ...)  Log.info(formatString,##__VA_ARGS__)

#define BT_CORE_LOG_WARN(formatString, ...)  Log.warn(formatString,##__VA_ARGS__)

#define BT_CORE_LOG_ERROR(formatString, ...) Log.error(formatString,##__VA_ARGS__)

namespace Bt {
namespace Core {

template <typename Enumeration>
auto asInteger(Enumeration const value) -> typename std::underlying_type<Enumeration>::type
{
    return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}

} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_Log__h
