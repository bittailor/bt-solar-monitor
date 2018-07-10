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

class MessageBuilderBase {
   public:
      static const size_t MESSAGE_BUFFER_LENGHT = 256;

   protected:
      MessageBuilderBase();
      void appendBegin();
      void appendEnd();

      void appendValues(float pFirst , float pSecond);
      void appendValues(int16_t pFirst , int16_t pSecond);

      size_t mMessageCounter;
      char mBuffer[MESSAGE_BUFFER_LENGHT];
      size_t mValuesCounter;
};

template<size_t NUMBER_OF_VALUES_IN_ONE_MESSAGE>
class MessageBuilder : MessageBuilderBase {
   public:
      static const size_t MAX_NUMBER_OF_VALUES_IN_ONE_MESSAGE = 60;
      static_assert(NUMBER_OF_VALUES_IN_ONE_MESSAGE <= MAX_NUMBER_OF_VALUES_IN_ONE_MESSAGE, "NUMBER_OF_VALUES_IN_ONE_MESSAGE must be smaller or equal to MAX_NUMBER_OF_VALUES_IN_ONE_MESSAGE" );

      template<size_t N>
      void append(std::array<float,N> pValues, std::function<void(const char* message)> pConsumer) {
         static_assert(N % 2 == 0, "N must be even");
         BT_CORE_LOG_INFO("MessageBuilder: append record %u of %u to Message", forPrintf(mValuesCounter/N), forPrintf(NUMBER_OF_VALUES_IN_ONE_MESSAGE / N));
         if(mValuesCounter==0) {
            appendBegin();
         }

         for (size_t i = 0; i < N; i+=2) {
            this->appendValues(pValues[i], pValues[i+1]);
         }
         mValuesCounter = mValuesCounter + N;

         if(mValuesCounter >= NUMBER_OF_VALUES_IN_ONE_MESSAGE) {
            appendEnd();
            BT_CORE_LOG_INFO("MessageBuilder: message ready");
            pConsumer(mBuffer);
            mMessageCounter++;
            mValuesCounter = 0;
         }
      }
};

template<size_t NUMBER_OF_VALUES_IN_ONE_RECORD, size_t NUMBER_OF_RECORDS>
class MessageFilter
{
   public:

      typedef std::function<void(const char* message)> Consumer;

      MessageFilter(const Consumer& pConsumer = Consumer())
      :mMessageCounter(0), mNumerOfStoredMeasurementRecords(0), mConsumer(pConsumer){
      }

      MessageFilter(const MessageFilter&) = delete;
      MessageFilter& operator=(const MessageFilter&) = delete;
      ~MessageFilter(){}

      void consume(const std::array<float,NUMBER_OF_VALUES_IN_ONE_RECORD>& pValues) {
         mMessage.append(pValues, mConsumer);
      }

      void link(const Consumer& pConsumer) {
         mConsumer = pConsumer;
      }

   private:

      size_t mMessageCounter;
      size_t mNumerOfStoredMeasurementRecords;
      MessageBuilder<NUMBER_OF_VALUES_IN_ONE_RECORD*NUMBER_OF_RECORDS> mMessage;
      Consumer mConsumer;

};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_MessageFilter__h
