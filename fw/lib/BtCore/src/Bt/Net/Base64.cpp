//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Net::Base64
//
//*************************************************************************************************

#include "Bt/Net/Base64.h"

#include <stdint.h>

namespace Bt {
namespace Net {
namespace Base64 {

namespace {
   const unsigned char BASE64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
}





int encode(const uint8_t* binaryData, size_t length, char* encoded)
{
   char* buffer = encoded;
   size_t i;
   for (i = 0; i < (length - 2) ; i += 3) {
      *buffer = BASE64[binaryData[i] >> 2];
      buffer++;
      *buffer = BASE64[((binaryData[i] & 0x03) << 4) + ((binaryData[i + 1] & 0xF0) >> 4)];
      buffer++;
      *buffer = BASE64[((binaryData[i + 1] & 0x0F) << 2) + ((binaryData[i + 2] & 0xC0) >> 6)];
      buffer++;
      *buffer = BASE64[binaryData[i + 2] & 0x3F];
      buffer++;
   }

   if (i < length) {
      *buffer = BASE64[(binaryData[i] >> 2) & 0x3F];
      buffer++;
      if (i == (length - 1)) {
         *buffer = BASE64[((binaryData[i] & 0x3) << 4)];
         buffer++;
         *buffer = '=';
         buffer++;
      }
      else {
         *buffer = BASE64[((binaryData[i] & 0x03) << 4) + ((binaryData[i + 1] & 0xF0) >> 4)];
         buffer++;
         *buffer = BASE64[((binaryData[i + 1] & 0xF) << 2)];
         buffer++;
      }
      *buffer = '=';
      buffer++;
   }

   *buffer = '\0';
   buffer++;
   return buffer - encoded;;
}


} // namespace Base64
} // namespace Net
} // namespace Bt
