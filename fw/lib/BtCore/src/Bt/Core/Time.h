//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::Time
//
//*************************************************************************************************

#ifndef INC__Bt_Core_Time__h
#define INC__Bt_Core_Time__h

#include "Bt/Core/I_Time.h"

namespace Bt {
namespace Core {

class Time : public I_Time
{
   public:
      Time();
      ~Time();

      virtual uint32_t seconds();
      virtual uint32_t milliseconds();
      virtual uint32_t microseconds();

   private:
        // Constructor to prohibit copy construction.
      Time(const Time&);

      // Operator= to prohibit copy assignment
      Time& operator=(const Time&);
};

} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_Time__h
