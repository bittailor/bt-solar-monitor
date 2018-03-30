//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Sensors::VeDirect::Utility
//
//*************************************************************************************************

#ifndef INC__Bt_Sensors_VeDirect_Utility__h
#define INC__Bt_Sensors_VeDirect_Utility__h

#include <stdint.h>
#include <stddef.h>
#include <string.h>

namespace Bt {
namespace Sensors {
namespace VeDirect {

uint8_t hexToByte(const char* pHex);
char* byteToHex(uint8_t pByte, char* pHex);

uint8_t checksum(const char* pHex);
uint8_t checksum(const char* pHex, size_t pLength);

template <typename T>
char* strAppendLittleEndianHex(char* pBuffer, T value) {
   const size_t numberOfBytes = sizeof(T);
   char* append = pBuffer + strlen(pBuffer);
   for (size_t i = 0; i < numberOfBytes; i++) {
      byteToHex((value >> (i * 8)),append);
      append+=2;
   }
   return pBuffer;
}


} // namespace VeDirect
} // namespace Sensors
} // namespace Bt

#endif // INC__Bt_Sensors_VeDirect_Utility__h
