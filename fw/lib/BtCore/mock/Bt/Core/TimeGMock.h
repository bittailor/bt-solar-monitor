//*************************************************************************************************
//
//  BITTAILOR.CH
//
//*************************************************************************************************

#ifndef INC__Bt_Core_TimeGMock__hpp
#define INC__Bt_Core_TimeGMock__hpp

#include <gmock/gmock.h>

#include "Bt/Core/I_Time.h"

namespace Bt {
namespace Core {

class TimeGMock : public I_Time {
   public:
      MOCK_METHOD0(seconds, uint32_t());
      MOCK_METHOD0(milliseconds, uint32_t());
      MOCK_METHOD0(microseconds, uint32_t());

};

} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_TimeGMock__hpp
