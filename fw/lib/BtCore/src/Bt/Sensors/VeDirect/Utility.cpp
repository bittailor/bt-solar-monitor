//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Sensors::VeDirect::Utility
//
//*************************************************************************************************

#include "Bt/Sensors/VeDirect/Utility.h"

#include <string.h>

namespace Bt {
namespace Sensors {
namespace VeDirect {
namespace {
   char toHex(int x) {
      int i = x & 0x000F;
      return i <= 9 ? '0' + i : 'A' - 10 + i;
   }
}


uint8_t hexToByte(const char* pHex) {
   uint8_t result = 0;
   for(size_t i=0; i<2; i++) {
      char c = *pHex;
      if (c >= '0' && c <= '9') {
         result *= 16;
         result += c - '0';
      }
      else if (c >= 'A' && c <= 'F') {
         result *= 16;
         result += (c - 'A') + 10;
      }
      else if (c >= 'a' && c <= 'f') {
         result *= 16;
         result += (c - 'a') + 10;
      }
      else if (c == ':') {
         result *= 16;
      }
      else {
         break;
      }
      pHex++;
   }
   return result;
}

char* byteToHex(uint8_t pByte, char* pHex) {
   pHex[0] = toHex((pByte & 0x00F0) >> 4);
   pHex[1] = toHex(pByte & 0x000F);
   pHex[2] = 0;
   return pHex;
}

uint8_t checksum(const char* pHex) {
   return checksum(pHex, strlen(pHex));
}

uint8_t checksum(const char* pHex, size_t pLength) {
   uint8_t checksum = 0x55;
   for (size_t i = 0; i < pLength; i+=2)
   {
      checksum -= hexToByte(pHex + i);;
   }
   return checksum;
}

} // namespace VeDirect
} // namespace Sensors
} // namespace Bt
