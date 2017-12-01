//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Sensors::VeDirect::Utility
//
//*************************************************************************************************

#ifndef INC__Bt_Sensors_VeDirect_Utility__h
#define INC__Bt_Sensors_VeDirect_Utility__h

#include <stdint.h>
#include <stddef.h>

namespace Bt {
namespace Sensors {
namespace VeDirect {

uint8_t hexToByte(const char* pHex);
char* byteToHex(uint8_t pByte, char* pHex);

uint8_t checksum(const char* pHex);
uint8_t checksum(const char* pHex, size_t pLength);

} // namespace VeDirect
} // namespace Sensors
} // namespace Bt

#endif // INC__Bt_Sensors_VeDirect_Utility__h
