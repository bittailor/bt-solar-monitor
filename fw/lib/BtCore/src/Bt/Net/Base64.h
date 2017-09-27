//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Net::Base64
//
//*************************************************************************************************

#ifndef INC__Bt_Net_Base64__h
#define INC__Bt_Net_Base64__h

#include <stddef.h>

namespace Bt {
namespace Net {
namespace Base64 {


constexpr int requiredEncodeSize(size_t pSize)
{
   return ((pSize + 2) / 3 * 4) + 1;
}

int encode(const char *string, int len, char *encoded);

} // namespace Base64
} // namespace Net
} // namespace Bt

#endif // INC__Bt_Net_Base64__h
