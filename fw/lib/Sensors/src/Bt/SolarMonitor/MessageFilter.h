//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::MessageFilter
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_MessageFilter__h
#define INC__Bt_SolarMonitor_MessageFilter__h

#include <array>
#include <functional>
#include <math.h>
#include "Bt/SolarMonitor/MeasurementRecord.h"

namespace Bt {
namespace SolarMonitor {

class Stream {
   public:
      Stream(char* pBuffer, size_t pCapacity)
      : mBuffer(pBuffer), mCapacity(pCapacity), mLenght(0){
         mBuffer[mLenght] = 0;
      }


      friend Stream& operator<<(Stream& pStream, int16_t pValue) {
         pStream.mBuffer[pStream.mLenght] = (pValue >> 8) & 0xff;
         pStream.mBuffer[pStream.mLenght+1] = pValue & 0xff;
         pStream.mLenght+=2;
         pStream.mBuffer[pStream.mLenght] = 0;
         return pStream;
      }

      friend Stream& operator<<(Stream& pStream, const Measurement& pMeasurement) {
         pStream << scale(pMeasurement.current()) << scale(pMeasurement.voltage());
         return pStream;
      }

   private:
      static int16_t scale(float value) {
        return static_cast<int16_t>(round(value * 1000));
     }

      char* mBuffer;
      size_t mCapacity;
      size_t mLenght;

};

template<size_t N, size_t R>
class MessageFilter
{
   public:

      typedef const char* MessageString;
      typedef std::function<void(const MessageString*, size_t size)> Consumer;

      MessageFilter(const Consumer& pConsumer = Consumer()):mConsumer(pConsumer){
      }
      MessageFilter(const MessageFilter&) = delete;
      MessageFilter& operator=(const MessageFilter&) = delete;
      ~MessageFilter(){}

      void consume(const std::array<MeasurementRecord<R>, N> pRecords) {
         size_t messageIndex = 0;
         Stream stream(mMessages[messageIndex],MESSAGE_BUFFER_LENGHT);

         for (const MeasurementRecord<R>& measurements : pRecords) {
            for (const Measurement& measurement : measurements) {
               stream << measurement;
            }
         }
      }

      void link(const Consumer& pConsumer) {
         mConsumer = pConsumer;
      }

   private:

      static const size_t ENTRIES_PER_MESSAGE = 6 * 10;
      static const size_t NUMBER_OF_MESSAGES = (N / 10) + ( N % 10 != 0 ? 1 : 0);
      static const size_t MESSAGE_BUFFER_LENGHT = (ENTRIES_PER_MESSAGE * (sizeof(int16_t) * 2)) + 1;

      typedef char Message[MESSAGE_BUFFER_LENGHT];

      std::array<Message,NUMBER_OF_MESSAGES> mMessages;
      Consumer mConsumer;

};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_MessageFilter__h
