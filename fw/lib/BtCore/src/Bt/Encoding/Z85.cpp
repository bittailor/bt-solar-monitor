//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Encoding::Z85
//
//*************************************************************************************************

#include "Bt/Encoding/Z85.h"

#include <assert.h>
#include "Bt/Core/Log.h"

namespace Bt {
namespace Encoding {
namespace Z85 {

namespace {
   const char encoder [85 + 1] = {
       "0123456789"
       "abcdefghij"
       "klmnopqrst"
       "uvwxyzABCD"
       "EFGHIJKLMN"
       "OPQRSTUVWX"
       "YZ.-:+=^!/"
       "*?&<>()[]{"
       "}@%$#"
   };
}

bool encode(const uint8_t* pData, size_t pSize, char* buffer, size_t pMaxBufferSize)
{
    //  Accepts only byte arrays bounded to 4 bytes
    if (pSize % 4) {
       BT_CORE_LOG_WARN("Z85::encode: input data array not bound to 4 bytes %u", pSize);
       return false;
    }

    size_t encodedSize = pSize * 5 / 4;

    if (pMaxBufferSize < encodedSize + 1) {
       BT_CORE_LOG_WARN("Z85::encode: output buffer too small %u < %u", pMaxBufferSize, encodedSize + 1);
       return false;
    }

    char *encoded = buffer;
    uint charNbr = 0;
    uint byteNbr = 0;
    uint32_t value = 0;
    while (byteNbr < pSize) {
        //  Accumulate value in base 256 (binary)
        value = value * 256 + pData[byteNbr++];
        if (byteNbr % 4 == 0) {
            //  Output value in base 85
            uint divisor = 85 * 85 * 85 * 85;
            while (divisor) {
                encoded[charNbr++] = encoder [value / divisor % 85];
                divisor /= 85;
            }
            value = 0;
        }
    }
    encoded[charNbr] = 0;
    return true;
}

} // namespace Z85
} // namespace Encoding
} // namespace Bt
