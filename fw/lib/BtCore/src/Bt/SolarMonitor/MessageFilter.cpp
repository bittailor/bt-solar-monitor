//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::MessageFilter
//
//*************************************************************************************************

#include "Bt/SolarMonitor/MessageFilter.h"

#include <string.h>

namespace Bt {
namespace SolarMonitor {

namespace {
   int16_t scale(float value) {
      return static_cast<int16_t>(round(value * 1000));
   }
}

Message::Message() {
   mBuffer[0] = 0;
   mCount = 0;
}

void Message::begin(size_t id, size_t i, size_t n) {
   mBuffer[0] = 0;
   mCount = 0;
   snprintf(mBuffer, MESSAGE_BUFFER_LENGHT, "%u|%u|%u|", forPrintf(id), forPrintf(i), forPrintf(n));
}

void Message::end() {
   BT_CORE_LOG_INFO("Message: Message::end() %u", forPrintf(strlen(mBuffer)));
   size_t length = strlen(mBuffer);
   snprintf(mBuffer + length, MESSAGE_BUFFER_LENGHT - length, "|%s",  Time.format(TIME_FORMAT_ISO8601_FULL).c_str());
}

bool Message::full() {
   return mCount >= 60;
}

const char* Message::raw() {
   return mBuffer;
}

void Message::append(float pFirst , float pSecond) {
   append(scale(pFirst), scale(pSecond));
}

void Message::append(int16_t pFirst , int16_t pSecond) {
   const size_t BUFFER_SIZE = 4;
   uint8_t buffer[BUFFER_SIZE];
   buffer[0] = (pFirst >> 8) & 0xff;
   buffer[1] = pFirst & 0xff;
   buffer[2] = (pSecond >> 8) & 0xff;
   buffer[3] = pSecond & 0xff;
   size_t length = strlen(mBuffer);
   Bt::Encoding::Z85::encode(buffer, BUFFER_SIZE, mBuffer + length, MESSAGE_BUFFER_LENGHT - length);
}

} // namespace SolarMonitor
} // namespace Bt
