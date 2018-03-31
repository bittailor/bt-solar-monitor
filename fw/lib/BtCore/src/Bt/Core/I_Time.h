//*************************************************************************************************
//
//  BITTAILOR.CH  -  Bt::Core::I_Time
//
//*************************************************************************************************

#ifndef INC__Bt_Core_I_Time__hpp
#define INC__Bt_Core_I_Time__hpp

#include <stdint.h>

namespace Bt {
namespace Core {

class I_Time {
   public:
      virtual ~I_Time() {}

      virtual uint32_t seconds() = 0;
      virtual uint32_t milliseconds() = 0;
      virtual uint32_t microseconds() = 0;
};

} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_I_Time__hpp
