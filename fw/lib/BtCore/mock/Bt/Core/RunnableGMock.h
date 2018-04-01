//*************************************************************************************************
//
//  BITTAILOR.CH
//
//*************************************************************************************************

#ifndef INC__Bt_Core_RunnableGMock__h
#define INC__Bt_Core_RunnableGMock__h

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

#endif // INC__Bt_Core_RunnableGMock__h
