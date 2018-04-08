//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::MessageFilter
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_MessageFilter__h
#define INC__Bt_SolarMonitor_MessageFilter__h

#include <array>
#include <functional>
#include <iterator>
#include <math.h>
#include "Bt/Core/Log.h"
#include "Bt/Encoding/Z85.h"
#include "Bt/SolarMonitor/MeasurementRecord.h"

namespace Bt {
namespace SolarMonitor {

class Message {
   public:
      static const size_t MESSAGE_BUFFER_LENGHT = 256;


      Message();
      void begin(size_t pCountMax, size_t id, size_t i, size_t n);

      template<size_t N>
      void append(std::array<float,N> pValues) {
         static_assert(N % 2 == 0, "N must be even");
         if(mCount == 0) {
            size_t length = strlen(mBuffer);
            snprintf(mBuffer + length, MESSAGE_BUFFER_LENGHT - length, "%s|",  Time.format(TIME_FORMAT_ISO8601_FULL).c_str());
         }
         for (size_t i = 0; i < N; i+=2) {
            append(pValues[i], pValues[i+1]);
         }
         mCount = mCount + N;
      }

      void end();
      bool full();
      const char* raw();

   private:
      void append(float pFirst , float pSecond);
      void append(int16_t pFirst , int16_t pSecond);

      size_t mCountMax;
      size_t mCount;
      char mBuffer[MESSAGE_BUFFER_LENGHT];
};

template<size_t N, size_t C>
class MessageFilter
{
   public:

      typedef const char* MessageString;
      typedef std::function<void(const MessageString*, size_t size)> Consumer;

      MessageFilter(const Consumer& pConsumer = Consumer())
      :mCurrentMessages(0), mMessageCounter(0), mNumerOfStoredMeasurementRecords(0), mCurrentMessageToAppend(0), mConsumer(pConsumer){
         mMessages[mCurrentMessages][0].begin(NUMBER_OF_VALUES_PER_MESSAGE, mMessageCounter, mCurrentMessageToAppend, NUMBER_OF_MESSAGES);
      }

      MessageFilter(const MessageFilter&) = delete;
      MessageFilter& operator=(const MessageFilter&) = delete;
      ~MessageFilter(){}

      void consume(const std::array<float,N>& pValues) {
         Message& message = mMessages[mCurrentMessages][mCurrentMessageToAppend];
         message.append(pValues);
         mNumerOfStoredMeasurementRecords++;
         BT_CORE_LOG_INFO("MessageFilter: append MeasurementRecord %u of %u to Message %u of %u [%u] :",
                          forPrintf(mNumerOfStoredMeasurementRecords), forPrintf(C) , forPrintf(mCurrentMessageToAppend), forPrintf(NUMBER_OF_MESSAGES), forPrintf(mCurrentMessages));
         if(message.full() || mNumerOfStoredMeasurementRecords >= C) {
            BT_CORE_LOG_INFO("MessageFilter: message %u of %u ready :", forPrintf(mCurrentMessageToAppend), forPrintf(NUMBER_OF_MESSAGES));
            message.end();
            mCurrentMessageToAppend++;
            if(mCurrentMessageToAppend >= NUMBER_OF_MESSAGES) {
               if(mConsumer) {
                  for (size_t i = 0; i < NUMBER_OF_MESSAGES; ++i) {
                     mConsumerMessages[i] = mMessages[mCurrentMessages][i].raw();
                  }
                  BT_CORE_LOG_INFO("MessageFilter: new messages %u ready [%u]:", forPrintf(NUMBER_OF_MESSAGES), forPrintf(mCurrentMessages));
                  for (size_t i = 0; i < NUMBER_OF_MESSAGES; ++i) {
                     BT_CORE_LOG_DEBUG("  - [%u]: %u", forPrintf(i), forPrintf(strlen(mConsumerMessages[i])));
                  }
                  mConsumer(mConsumerMessages, NUMBER_OF_MESSAGES);
               }
               mCurrentMessages = mCurrentMessages == 0 ? 1 : 0;
               mMessageCounter++;
               mCurrentMessageToAppend = 0;
               mNumerOfStoredMeasurementRecords = 0;
            }
            mMessages[mCurrentMessages][mCurrentMessageToAppend].begin(NUMBER_OF_VALUES_PER_MESSAGE, mMessageCounter, mCurrentMessageToAppend, NUMBER_OF_MESSAGES);
         }

      }

      void link(const Consumer& pConsumer) {
         mConsumer = pConsumer;
      }


      static const size_t NUMBER_OF_MEASUREMENTRECORD_PER_MESSAGE = 6;
      static const size_t NUMBER_OF_VALUES_PER_MESSAGE = NUMBER_OF_MEASUREMENTRECORD_PER_MESSAGE *  N;

      static const size_t NUMBER_OF_MESSAGES = ( C / NUMBER_OF_MEASUREMENTRECORD_PER_MESSAGE) +
                                               ( C % NUMBER_OF_MEASUREMENTRECORD_PER_MESSAGE != 0 ? 1 : 0); // +1;
   private:

      //typedef char MessageBuffer[MESSAGE_BUFFER_LENGHT];
      size_t mCurrentMessages;
      size_t mMessageCounter;
      size_t mNumerOfStoredMeasurementRecords;
      size_t mCurrentMessageToAppend;
      MessageString mConsumerMessages[NUMBER_OF_MESSAGES];
      std::array<std::array<Message,NUMBER_OF_MESSAGES>, 2> mMessages;
      Consumer mConsumer;

};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_MessageFilter__h
