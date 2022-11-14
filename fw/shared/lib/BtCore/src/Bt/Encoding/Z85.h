//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Encoding::Z85
//
//*************************************************************************************************

#ifndef INC__Bt_Encoding_Z85__h
#define INC__Bt_Encoding_Z85__h

#include <stddef.h>
#include <stdint.h>

namespace Bt {
namespace Encoding {
namespace Z85 {

constexpr int requiredEncodeBufferSize(size_t pSize)
{
   return (pSize * 5 / 4) + 1;
}

/**
 * see https://rfc.zeromq.org/spec:32/Z85/
 */

bool encode(const uint8_t* pData, size_t pSize, char* buffer, size_t pMaxBufferSize);
size_t decode(const char* pString, uint8_t* pBuffer, size_t pMaxBufferSize);

} // namespace Z85
} // namespace Encoding
} // namespace Bt

#endif // INC__Bt_Encoding_Z85__h
