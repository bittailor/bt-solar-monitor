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

MessageBuilderBase::MessageBuilderBase(): mMessageCounter(0), mValuesCounter(0) {
   mBuffer[0] = 0;
}

void MessageBuilderBase::appendBegin() {
   mBuffer[0] = 0;
   snprintf(mBuffer, MESSAGE_BUFFER_LENGHT, "%u|%u|%u|%s|", forPrintf(mMessageCounter), 0, 0, Time.format(TIME_FORMAT_ISO8601_FULL).c_str());
}

void MessageBuilderBase::appendEnd() {
   BT_CORE_LOG_INFO("Message: Message::end() %u", forPrintf(strlen(mBuffer)));
   size_t length = strlen(mBuffer);
   snprintf(mBuffer + length, MESSAGE_BUFFER_LENGHT - length, "|%s",  Time.format(TIME_FORMAT_ISO8601_FULL).c_str());
}

void MessageBuilderBase::appendValues(float pFirst , float pSecond) {
   appendValues(scale(pFirst), scale(pSecond));
}

void MessageBuilderBase::appendValues(int16_t pFirst , int16_t pSecond) {
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
