//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Encoding::Base64
//
//*************************************************************************************************

#ifndef INC__Bt_Encoding_Base64__h
#define INC__Bt_Encoding_Base64__h

#include <stddef.h>

namespace Bt {
namespace Encoding {
namespace Base64 {


constexpr int requiredEncodeSize(size_t pSize)
{
   return ((pSize + 2) / 3 * 4) + 1;
}

int encode(const char *string, int len, char *encoded);

} // namespace Base64
} // namespace Encoding
} // namespace Bt

#endif // INC__Bt_Encoding_Base64__h
