//*************************************************************************************************
//
//  BITTAILOR.CH  -  Bt::Core::I_Workcycle
//
//*************************************************************************************************

#ifndef INC__Bt_Core_I_Workcycle__h
#define INC__Bt_Core_I_Workcycle__h

#include <type_traits>

#include "Bt/Core/I_Runnable.h"

namespace Bt {
namespace Core {

class I_Workcycle {
   public:
      virtual ~I_Workcycle() {}

      virtual void add(I_Runnable& iRunnable) = 0;
      virtual void remove(I_Runnable& iRunnable) = 0;

   private:
      
};

} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_I_Workcycle__h
