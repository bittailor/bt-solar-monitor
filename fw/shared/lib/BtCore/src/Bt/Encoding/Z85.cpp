//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Encoding::Z85
//
//*************************************************************************************************

#include "Bt/Encoding/Z85.h"

#include <string.h>

//#include "Bt/Core/Log.h"

namespace Bt {
namespace Encoding {
namespace Z85 {

namespace {
const char ENCODER [85 + 1] = {
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

const uint8_t DECODER [96] = {
    0x00, 0x44, 0x00, 0x54, 0x53, 0x52, 0x48, 0x00,
    0x4B, 0x4C, 0x46, 0x41, 0x00, 0x3F, 0x3E, 0x45,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x40, 0x00, 0x49, 0x42, 0x4A, 0x47,
    0x51, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A,
    0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32,
    0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A,
    0x3B, 0x3C, 0x3D, 0x4D, 0x00, 0x4E, 0x43, 0x00,
    0x00, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10,
    0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
    0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20,
    0x21, 0x22, 0x23, 0x4F, 0x00, 0x50, 0x00, 0x00
};

}

bool encode(const uint8_t* pData, size_t pSize, char* pBuffer, size_t pMaxBufferSize)
{
   if (pSize % 4) {
      //BT_CORE_LOG_WARN("Z85::encode: input data array not bound to 4 bytes %u", pSize);
      return false;
   }

   size_t encodedSize = pSize * 5 / 4;

   if (pMaxBufferSize < encodedSize + 1) {
      //BT_CORE_LOG_WARN("Z85::encode: output buffer too small %u < %u", pMaxBufferSize, encodedSize + 1);
      return false;
   }

   char *encoded = pBuffer;
   size_t charNbr = 0;
   size_t byteNbr = 0;
   uint32_t value = 0;
   while (byteNbr < pSize) {
      value = value * 256 + pData[byteNbr++];
      if (byteNbr % 4 == 0) {
         size_t divisor = 85 * 85 * 85 * 85;
         while (divisor) {
            encoded[charNbr++] = ENCODER[value / divisor % 85];
            divisor /= 85;
         }
         value = 0;
      }
   }
   encoded[charNbr] = 0;
   return true;
}

size_t decode(const char* pString, uint8_t* pBuffer, size_t pMaxBufferSize) {
   size_t stringLength = strlen(pString);
   if(stringLength % 5 != 0) {
      return 0;
   }

   size_t decodedSize = stringLength * 4 / 5;
   if(decodedSize > pMaxBufferSize) {
      return 0;
   }

   size_t byteNbr = 0;
   size_t charNbr = 0;
   uint32_t value = 0;
   while (charNbr < stringLength) {
      value = value * 85 + DECODER[(uint8_t) pString[charNbr++] - 32];
      if (charNbr % 5 == 0) {
         size_t divisor = 256 * 256 * 256;
         while (divisor) {
            pBuffer[byteNbr++] = value / divisor % 256;
            divisor /= 256;
         }
         value = 0;
      }
   }
   return decodedSize;
}

} // namespace Z85
} // namespace Encoding
} // namespace Bt
