//*************************************************************************************************
//
//  BITTAILOR.CH
//
//*************************************************************************************************

#ifndef INC__Bt_Core_RunnableGMock__hpp
#define INC__Bt_Core_RunnableGMock__hpp

#include <gmock/gmock.h>

#include "Bt/Core/I_Runnable.h"

namespace Bt {
namespace Core {

class RunnableGMock : public I_Runnable {
   public:
      MOCK_METHOD0(workcycle,Scheduling());
};

} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_RunnableGMock__hpp
