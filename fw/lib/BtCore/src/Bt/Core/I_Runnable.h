//*************************************************************************************************
//
//  BITTAILOR.CH  -  Bt::Core::I_Runnable
//
//*************************************************************************************************

#ifndef INC__Bt_Core_I_Runnable__h
#define INC__Bt_Core_I_Runnable__h

#include <Bt/Core/Scheduling.h>
#include <Bt/Core/IntrusiveList.h>

namespace Bt {
namespace Core {

class I_Runnable : public IntrusiveList<I_Runnable>::Element {
   public:

      virtual ~I_Runnable() {}
      virtual Scheduling workcycle() = 0;
};

} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_I_Runnable__h
