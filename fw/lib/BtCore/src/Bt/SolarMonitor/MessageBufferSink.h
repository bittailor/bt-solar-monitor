   //*************************************************************************************************
//
//  BITTAILOR.CH - Bt::SolarMonitor::MessageBufferSink
//
//*************************************************************************************************

#ifndef INC__Bt_SolarMonitor_MessageBufferSink__h
#define INC__Bt_SolarMonitor_MessageBufferSink__h

#include <array>
#include <string.h>

#include <Bt/Core/Log.h>
#include <Bt/Core/CircularBuffer.h>
#include "Bt/SolarMonitor/I_MessageBuffer.h"

namespace Bt {
namespace SolarMonitor {

class Message {
   public:
      static const size_t MESSAGE_BUFFER_LENGHT = 256;

      Message() {
         mBuffer[0] = 0;
      }

      Message(const char* pMessage) {
         strncpy(mBuffer, pMessage, MESSAGE_BUFFER_LENGHT);
      }

      Message(const Message& pOther) {
         strncpy(mBuffer, pOther.mBuffer, MESSAGE_BUFFER_LENGHT);
      }

      Message& operator=(const Message& pOther) {
         if(this != &pOther) {
            strncpy(mBuffer, pOther.mBuffer, MESSAGE_BUFFER_LENGHT);
         }
         return *this;
      }

      const char* message() const {
         return mBuffer;
      }

   private:
      char mBuffer[MESSAGE_BUFFER_LENGHT];
};

template<size_t SIZE>
class MessageBufferSink : public I_MessageBuffer
{
   public:
      MessageBufferSink() = default;
      MessageBufferSink(const MessageBufferSink&) = delete;
      MessageBufferSink& operator=(const MessageBufferSink&) = delete;

      void consume(const char* pMessage){
         mCircularBuffer.push(pMessage);
      }

      virtual const char* peak() const {
         const Message* message = mCircularBuffer.peak();
         return message ? message->message() : nullptr ;
      }

      virtual void pop()
      {
         mCircularBuffer.pop();
      }

      virtual bool empty() const
      {
         return mCircularBuffer.empty();
      }

      virtual size_t count() const {
         return mCircularBuffer.count();
      }


   private:

      Core::CircularBuffer<Message, SIZE> mCircularBuffer;
};

} // namespace SolarMonitor
} // namespace Bt

#endif // INC__Bt_SolarMonitor_MessageBufferSink__h
